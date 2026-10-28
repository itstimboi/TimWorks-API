
#include <TWAPI/TW_audio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

/*
    Enable OGG Vorbis decoding through stb_vorbis.
    Keep this include order.
*/
#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#undef STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

/* ---------------------------------------------------------
   Internal audio object
   --------------------------------------------------------- */

struct TWAPI_Audio
{
    ma_sound sound;
    ma_decoder decoder;

    int initialized;
    int decoderInitialized;
    int paused;

    ma_uint64 pausedFrame;

    unsigned char* ownedData;
    size_t ownedDataSize;

    float volume;
    float pan;
    int looping;

    int spatialEnabled;
    int spatialPositionInitialized;

    TWAPI_Vector3 targetPosition;
    TWAPI_Vector3 currentPosition;
    TWAPI_Vector3 velocity;

    float minDistance;
    float maxDistance;
    float dopplerFactor;

    struct TWAPI_Audio* next;
};

/* ---------------------------------------------------------
   Global engine state
   --------------------------------------------------------- */

static ma_engine g_audioEngine;
static int g_audioInitialized = 0;

static TWAPI_Audio* g_audioList = NULL;

static TWAPI_Vector3 g_listenerPosition =
    { 0.0f, 0.0f, 0.0f };

static TWAPI_Vector3 g_listenerForward =
    { 0.0f, 0.0f, -1.0f };

static TWAPI_Vector3 g_listenerUp =
    { 0.0f, 1.0f, 0.0f };

/* ---------------------------------------------------------
   Vector helpers
   --------------------------------------------------------- */

static float tw_clamp(float value, float minimum, float maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

static float tw_dot(TWAPI_Vector3 a, TWAPI_Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static TWAPI_Vector3 tw_sub(
    TWAPI_Vector3 a,
    TWAPI_Vector3 b)
{
    TWAPI_Vector3 result = {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };

    return result;
}

static TWAPI_Vector3 tw_scale(
    TWAPI_Vector3 v,
    float scale)
{
    TWAPI_Vector3 result = {
        v.x * scale,
        v.y * scale,
        v.z * scale
    };

    return result;
}

static TWAPI_Vector3 tw_add(
    TWAPI_Vector3 a,
    TWAPI_Vector3 b)
{
    TWAPI_Vector3 result = {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };

    return result;
}

static float tw_length(TWAPI_Vector3 v)
{
    return sqrtf(tw_dot(v, v));
}

static int tw_normalize(TWAPI_Vector3* v)
{
    float length = tw_length(*v);

    if (length < 0.00001f)
        return 0;

    *v = tw_scale(*v, 1.0f / length);
    return 1;
}

static TWAPI_Vector3 tw_lerp(
    TWAPI_Vector3 a,
    TWAPI_Vector3 b,
    float amount)
{
    return tw_add(
        a,
        tw_scale(tw_sub(b, a), amount)
    );
}

/* ---------------------------------------------------------
   Object tracking
   --------------------------------------------------------- */

static void tw_register_audio(TWAPI_Audio* audio)
{
    audio->next = g_audioList;
    g_audioList = audio;
}

static void tw_unregister_audio(TWAPI_Audio* audio)
{
    TWAPI_Audio** current = &g_audioList;

    while (*current)
    {
        if (*current == audio)
        {
            *current = audio->next;
            return;
        }

        current = &(*current)->next;
    }
}

/* ---------------------------------------------------------
   Defaults and sound properties
   --------------------------------------------------------- */

static TWAPI_Audio* tw_allocate_audio(void)
{
    TWAPI_Audio* audio =
        (TWAPI_Audio*)calloc(1, sizeof(TWAPI_Audio));

    if (!audio)
        return NULL;

    audio->volume = 1.0f;
    audio->pan = 0.0f;
    audio->looping = 0;

    audio->spatialEnabled = 0;
    audio->spatialPositionInitialized = 0;

    audio->targetPosition =
        (TWAPI_Vector3){ 0.0f, 0.0f, 0.0f };

    audio->currentPosition =
        (TWAPI_Vector3){ 0.0f, 0.0f, 0.0f };

    audio->velocity =
        (TWAPI_Vector3){ 0.0f, 0.0f, 0.0f };

    audio->minDistance = 1.0f;
    audio->maxDistance = 20.0f;
    audio->dopplerFactor = 1.0f;

    return audio;
}

static void tw_apply_basic_properties(TWAPI_Audio* audio)
{
    if (!audio || !audio->initialized)
        return;

    ma_sound_set_volume(&audio->sound, audio->volume);
    ma_sound_set_pan(&audio->sound, audio->pan);
    ma_sound_set_looping(&audio->sound,
        audio->looping ? MA_TRUE : MA_FALSE);
}

static void tw_apply_spatial_properties(TWAPI_Audio* audio)
{
    if (!audio || !audio->initialized)
        return;

    ma_sound_set_spatialization_enabled(
        &audio->sound,
        audio->spatialEnabled ? MA_TRUE : MA_FALSE
    );

    if (!audio->spatialEnabled)
        return;

    ma_sound_set_position(
        &audio->sound,
        audio->currentPosition.x,
        audio->currentPosition.y,
        audio->currentPosition.z
    );

    ma_sound_set_velocity(
        &audio->sound,
        audio->velocity.x,
        audio->velocity.y,
        audio->velocity.z
    );

    ma_sound_set_min_distance(
        &audio->sound,
        audio->minDistance
    );

    ma_sound_set_max_distance(
        &audio->sound,
        audio->maxDistance
    );

    ma_sound_set_attenuation_model(
        &audio->sound,
        ma_attenuation_model_linear
    );

    ma_sound_set_doppler_factor(
        &audio->sound,
        audio->dopplerFactor
    );
}

/* ---------------------------------------------------------
   Initialization and shutdown
   --------------------------------------------------------- */

int TWAPI_AudioInit(void)
{
    if (g_audioInitialized)
        return 1;

    ma_engine_config config = ma_engine_config_init();

    if (ma_engine_init(&config, &g_audioEngine) != MA_SUCCESS)
        return 0;

    g_audioInitialized = 1;

    ma_engine_listener_set_position(
        &g_audioEngine, 0,
        g_listenerPosition.x,
        g_listenerPosition.y,
        g_listenerPosition.z
    );

    ma_engine_listener_set_direction(
        &g_audioEngine, 0,
        g_listenerForward.x,
        g_listenerForward.y,
        g_listenerForward.z
    );

    ma_engine_listener_set_world_up(
        &g_audioEngine, 0,
        g_listenerUp.x,
        g_listenerUp.y,
        g_listenerUp.z
    );

    return 1;
}

void TWAPI_AudioShutdown(void)
{
    if (!g_audioInitialized)
        return;

    while (g_audioList)
        TWAPI_FreeAudio(g_audioList);

    ma_engine_uninit(&g_audioEngine);

    g_audioInitialized = 0;
}

/* ---------------------------------------------------------
   Loading from files
   --------------------------------------------------------- */

TWAPI_Audio* TWAPI_LoadAudio(const char* filename)
{
    if (!g_audioInitialized || !filename || !filename[0])
        return NULL;

    TWAPI_Audio* audio = tw_allocate_audio();

    if (!audio)
        return NULL;

    ma_result result = ma_sound_init_from_file(
        &g_audioEngine,
        filename,
        MA_SOUND_FLAG_DECODE,
        NULL,
        NULL,
        &audio->sound
    );

    if (result != MA_SUCCESS)
    {
        free(audio);
        return NULL;
    }

    audio->initialized = 1;

    /*
        Ordinary sounds start in 2D mode.
        TWAPI_SetAudioPosition enables spatialization.
    */
    ma_sound_set_spatialization_enabled(
        &audio->sound,
        MA_FALSE
    );

    tw_apply_basic_properties(audio);
    tw_register_audio(audio);

    return audio;
}

/* ---------------------------------------------------------
   Loading from encoded memory
   --------------------------------------------------------- */

TWAPI_Audio* TWAPI_LoadAudioFromMemory(
    const void* bytes,
    size_t size)
{
    if (!g_audioInitialized || !bytes || size == 0)
        return NULL;

    TWAPI_Audio* audio = tw_allocate_audio();

    if (!audio)
        return NULL;

    audio->ownedData = (unsigned char*)malloc(size);

    if (!audio->ownedData)
    {
        free(audio);
        return NULL;
    }

    memcpy(audio->ownedData, bytes, size);
    audio->ownedDataSize = size;

    ma_result result = ma_decoder_init_memory(
        audio->ownedData,
        audio->ownedDataSize,
        NULL,
        &audio->decoder
    );

    if (result != MA_SUCCESS)
    {
        free(audio->ownedData);
        free(audio);
        return NULL;
    }

    audio->decoderInitialized = 1;

    result = ma_sound_init_from_data_source(
        &g_audioEngine,
        &audio->decoder,
        0,
        NULL,
        &audio->sound
    );

    if (result != MA_SUCCESS)
    {
        ma_decoder_uninit(&audio->decoder);
        free(audio->ownedData);
        free(audio);
        return NULL;
    }

    audio->initialized = 1;

    ma_sound_set_spatialization_enabled(
        &audio->sound,
        MA_FALSE
    );

    tw_apply_basic_properties(audio);
    tw_register_audio(audio);

    return audio;
}

/* ---------------------------------------------------------
   Base64 decoder
   --------------------------------------------------------- */

static int tw_base64_value(unsigned char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;

    return -1;
}

static int tw_is_base64_space(unsigned char c)
{
    return c == ' ' || c == '\t' ||
           c == '\r' || c == '\n';
}

static unsigned char* tw_decode_base64(
    const char* input,
    size_t* outputSize)
{
    if (!input || !outputSize)
        return NULL;

    *outputSize = 0;

    size_t inputLength = strlen(input);

    if (inputLength == 0)
        return NULL;

    if (inputLength > SIZE_MAX - 3)
        return NULL;

    size_t capacity = ((inputLength + 3) / 4) * 3;

    if (capacity == 0)
        return NULL;

    unsigned char* output =
        (unsigned char*)malloc(capacity);

    if (!output)
        return NULL;

    unsigned char quartet[4];
    int quartetCount = 0;
    int paddingSeen = 0;

    size_t out = 0;

    for (size_t i = 0; i < inputLength; ++i)
    {
        unsigned char c = (unsigned char)input[i];

        if (tw_is_base64_space(c))
            continue;

        if (paddingSeen && c != '=')
        {
            free(output);
            return NULL;
        }

        if (c == '=')
        {
            quartet[quartetCount++] = 64;
            paddingSeen = 1;
        }
        else
        {
            int value = tw_base64_value(c);

            if (value < 0)
            {
                free(output);
                return NULL;
            }

            quartet[quartetCount++] =
                (unsigned char)value;
        }

        if (quartetCount == 4)
        {
            if (quartet[0] == 64 || quartet[1] == 64)
            {
                free(output);
                return NULL;
            }

            if (quartet[2] == 64 && quartet[3] != 64)
            {
                free(output);
                return NULL;
            }

            uint32_t value =
                ((uint32_t)quartet[0] << 18) |
                ((uint32_t)quartet[1] << 12) |
                ((uint32_t)(quartet[2] & 63) << 6) |
                (uint32_t)(quartet[3] & 63);

            output[out++] = (unsigned char)(value >> 16);

            if (quartet[2] != 64)
                output[out++] = (unsigned char)(value >> 8);

            if (quartet[3] != 64)
                output[out++] = (unsigned char)value;

            quartetCount = 0;
        }
    }

    if (quartetCount != 0)
    {
        free(output);
        return NULL;
    }

    if (out == 0)
    {
        free(output);
        return NULL;
    }

    *outputSize = out;
    return output;
}

TWAPI_Audio* TWAPI_LoadAudioFromBase64(const char* base64)
{
    size_t decodedSize = 0;

    unsigned char* decoded =
        tw_decode_base64(base64, &decodedSize);

    if (!decoded)
        return NULL;

    TWAPI_Audio* audio =
        TWAPI_LoadAudioFromMemory(decoded, decodedSize);

    free(decoded);

    return audio;
}

/* ---------------------------------------------------------
   Releasing an audio object
   --------------------------------------------------------- */

void TWAPI_FreeAudio(TWAPI_Audio* audio)
{
    if (!audio)
        return;

    tw_unregister_audio(audio);

    if (audio->initialized)
    {
        ma_sound_stop(&audio->sound);
        ma_sound_uninit(&audio->sound);
        audio->initialized = 0;
    }

    if (audio->decoderInitialized)
    {
        ma_decoder_uninit(&audio->decoder);
        audio->decoderInitialized = 0;
    }

    free(audio->ownedData);
    audio->ownedData = NULL;

    free(audio);
}

/* ---------------------------------------------------------
   Playback controls
   --------------------------------------------------------- */

int TWAPI_PlayAudio(TWAPI_Audio* audio)
{
    if (!audio || !audio->initialized)
        return 0;

    ma_sound_stop(&audio->sound);

    if (ma_sound_seek_to_pcm_frame(&audio->sound, 0)
        != MA_SUCCESS)
    {
        return 0;
    }

    audio->paused = 0;
    audio->pausedFrame = 0;

    return ma_sound_start(&audio->sound) == MA_SUCCESS;
}

void TWAPI_StopAudio(TWAPI_Audio* audio)
{
    if (!audio || !audio->initialized)
        return;

    ma_sound_stop(&audio->sound);
    ma_sound_seek_to_pcm_frame(&audio->sound, 0);

    audio->paused = 0;
    audio->pausedFrame = 0;
}

void TWAPI_PauseAudio(TWAPI_Audio* audio)
{
    if (!audio || !audio->initialized || audio->paused)
        return;

    if (!ma_sound_is_playing(&audio->sound))
        return;

    ma_uint64 cursor = 0;

    if (ma_sound_get_cursor_in_pcm_frames(
            &audio->sound, &cursor) != MA_SUCCESS)
    {
        return;
    }

    audio->pausedFrame = cursor;
    ma_sound_stop(&audio->sound);
    audio->paused = 1;
}

void TWAPI_ResumeAudio(TWAPI_Audio* audio)
{
    if (!audio || !audio->initialized || !audio->paused)
        return;

    if (ma_sound_seek_to_pcm_frame(
            &audio->sound, audio->pausedFrame) != MA_SUCCESS)
    {
        return;
    }

    audio->paused = 0;

    if (ma_sound_start(&audio->sound) != MA_SUCCESS)
        audio->paused = 1;
}

int TWAPI_IsAudioPlaying(const TWAPI_Audio* audio)
{
    if (!audio || !audio->initialized || audio->paused)
        return 0;

    return ma_sound_is_playing(
        (ma_sound*)&audio->sound) ? 1 : 0;
}

int TWAPI_RewindAudio(TWAPI_Audio* audio)
{
    if (!audio || !audio->initialized)
        return 0;

    audio->paused = 0;
    audio->pausedFrame = 0;

    return ma_sound_seek_to_pcm_frame(
        &audio->sound, 0) == MA_SUCCESS;
}

/* ---------------------------------------------------------
   Basic sound properties
   --------------------------------------------------------- */

void TWAPI_SetAudioLooping(TWAPI_Audio* audio, int looping)
{
    if (!audio || !audio->initialized)
        return;

    audio->looping = looping != 0;

    ma_sound_set_looping(
        &audio->sound,
        audio->looping ? MA_TRUE : MA_FALSE
    );
}

void TWAPI_SetAudioVolume(TWAPI_Audio* audio, float volume)
{
    if (!audio || !audio->initialized)
        return;

    audio->volume = tw_clamp(volume, 0.0f, 1.0f);

    ma_sound_set_volume(&audio->sound, audio->volume);
}

float TWAPI_GetAudioVolume(const TWAPI_Audio* audio)
{
    return audio ? audio->volume : 0.0f;
}

void TWAPI_SetAudioPan(TWAPI_Audio* audio, float pan)
{
    if (!audio || !audio->initialized)
        return;

    audio->pan = tw_clamp(pan, -1.0f, 1.0f);

    ma_sound_set_pan(&audio->sound, audio->pan);
}

float TWAPI_GetAudioPan(const TWAPI_Audio* audio)
{
    return audio ? audio->pan : 0.0f;
}

/* ---------------------------------------------------------
   Listener position and orientation
   --------------------------------------------------------- */

void TWAPI_SetAudioListener(
    TWAPI_Vector3 position,
    TWAPI_Vector3 forward,
    TWAPI_Vector3 up)
{
    if (!g_audioInitialized)
        return;

    if (!tw_normalize(&forward))
        return;

    /*
        Make up perpendicular to forward.
    */
    up = tw_sub(up, tw_scale(forward, tw_dot(up, forward)));

    if (!tw_normalize(&up))
        return;

    g_listenerPosition = position;
    g_listenerForward = forward;
    g_listenerUp = up;

    ma_engine_listener_set_position(
        &g_audioEngine, 0,
        position.x, position.y, position.z
    );

    ma_engine_listener_set_direction(
        &g_audioEngine, 0,
        forward.x, forward.y, forward.z
    );

    ma_engine_listener_set_world_up(
        &g_audioEngine, 0,
        up.x, up.y, up.z
    );
}

/* ---------------------------------------------------------
   3D sound properties
   --------------------------------------------------------- */

void TWAPI_SetAudioPosition(
    TWAPI_Audio* audio,
    TWAPI_Vector3 position)
{
    if (!audio || !audio->initialized)
        return;

    audio->targetPosition = position;

    /*
        Avoid a sudden glide from the origin the first
        time the sound is assigned a world position.
    */
    if (!audio->spatialPositionInitialized)
    {
        audio->currentPosition = position;
        audio->spatialPositionInitialized = 1;
    }

    audio->spatialEnabled = 1;

    tw_apply_spatial_properties(audio);
}

void TWAPI_SetAudioDistance(
    TWAPI_Audio* audio,
    float minDistance,
    float maxDistance)
{
    if (!audio || !audio->initialized)
        return;

    if (minDistance < 0.0f)
        minDistance = 0.0f;

    if (maxDistance <= minDistance)
        return;

    audio->minDistance = minDistance;
    audio->maxDistance = maxDistance;

    if (audio->spatialEnabled)
        tw_apply_spatial_properties(audio);
}

void TWAPI_SetAudioVelocity(
    TWAPI_Audio* audio,
    TWAPI_Vector3 velocity)
{
    if (!audio || !audio->initialized)
        return;

    audio->velocity = velocity;

    if (audio->spatialEnabled)
    {
        ma_sound_set_velocity(
            &audio->sound,
            velocity.x,
            velocity.y,
            velocity.z
        );
    }
}

void TWAPI_SetAudioDopplerFactor(
    TWAPI_Audio* audio,
    float factor)
{
    if (!audio || !audio->initialized)
        return;

    audio->dopplerFactor = tw_clamp(factor, 0.0f, 10.0f);

    if (audio->spatialEnabled)
    {
        ma_sound_set_doppler_factor(
            &audio->sound,
            audio->dopplerFactor
        );
    }
}

void TWAPI_DisableAudio3D(TWAPI_Audio* audio)
{
    if (!audio || !audio->initialized)
        return;

    audio->spatialEnabled = 0;

    ma_sound_set_spatialization_enabled(
        &audio->sound,
        MA_FALSE
    );

    /*
        Restore the ordinary 2D volume and pan.
    */
    tw_apply_basic_properties(audio);
}

/* ---------------------------------------------------------
   Smooth spatial updates
   --------------------------------------------------------- */

void TWAPI_UpdateAudio(float deltaTime)
{
    if (!g_audioInitialized || deltaTime <= 0.0f)
        return;

    /*
        Exponential smoothing:
        higher speed = faster response.
    */
    const float smoothingSpeed = 14.0f;

    if (deltaTime > 0.25f)
        deltaTime = 0.25f;

    float blend =
        1.0f - expf(-smoothingSpeed * deltaTime);

    blend = tw_clamp(blend, 0.0f, 1.0f);

    for (TWAPI_Audio* audio = g_audioList;
         audio != NULL;
         audio = audio->next)
    {
        if (!audio->initialized ||
            !audio->spatialEnabled ||
            !audio->spatialPositionInitialized)
        {
            continue;
        }

        audio->currentPosition = tw_lerp(
            audio->currentPosition,
            audio->targetPosition,
            blend
        );

        ma_sound_set_position(
            &audio->sound,
            audio->currentPosition.x,
            audio->currentPosition.y,
            audio->currentPosition.z
        );
    }
}

TWAPI_Audio* TWAPI_LoadAudioStream(const char* filename)
{
    if (!g_audioInitialized || !filename || !filename[0])
        return NULL;

    TWAPI_Audio* audio = tw_allocate_audio();

    if (!audio)
        return NULL;

    /*
        MA_SOUND_FLAG_STREAM tells miniaudio to stream the
        file instead of loading the entire track into memory.
    */
    ma_result result = ma_sound_init_from_file(
        &g_audioEngine,
        filename,
        MA_SOUND_FLAG_STREAM,
        NULL,
        NULL,
        &audio->sound
    );

    if (result != MA_SUCCESS)
    {
        free(audio);
        return NULL;
    }

    audio->initialized = 1;

    ma_sound_set_spatialization_enabled(
        &audio->sound,
        MA_FALSE
    );

    tw_apply_basic_properties(audio);
    tw_register_audio(audio);

    return audio;
}
