#include "terrarium.h"

using namespace terrarium;

#ifndef SAMPLE_RATE
//#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE
#define SAMPLE_RATE 48014.f
#endif

// Hardware related defines.
// Switches
constexpr Pin SW_1_PIN = seed::D10;
constexpr Pin SW_2_PIN = seed::D9;
constexpr Pin SW_3_PIN = seed::D8;
constexpr Pin SW_4_PIN = seed::D7;

// Footswitches
constexpr Pin FTSW_1_PIN = seed::D25;
constexpr Pin FTSW_2_PIN = seed::D26;

// Pots
constexpr Pin POT_1_PIN     = seed::D16;
constexpr Pin POT_2_PIN     = seed::D17;
constexpr Pin POT_3_PIN     = seed::D18;
constexpr Pin POT_4_PIN     = seed::D19;
constexpr Pin POT_5_PIN     = seed::D20;
constexpr Pin POT_6_PIN     = seed::D21;

constexpr Pin LED_1_PIN     = seed::D22;
constexpr Pin LED_2_PIN     = seed::D23;

void Terrarium::Init(bool boost)
{
    // Set Some numbers up for accessors.
    // Initialize the hardware.
    seed.Configure();
    seed.Init(boost);
    InitSwitches();
    InitLeds();
    InitAnalogControls();
    SetAudioBlockSize(48);
}

void Terrarium::DelayMs(size_t del)
{
    seed.DelayMs(del);
}

void Terrarium::StartAudio(AudioHandle::InterleavingAudioCallback cb)
{
    seed.StartAudio(cb);
}

void Terrarium::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void Terrarium::ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void Terrarium::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void Terrarium::StopAudio()
{
    seed.StopAudio();
}

void Terrarium::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
    SetHidUpdateRates();
}

float Terrarium::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

void Terrarium::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
    SetHidUpdateRates();
}

size_t Terrarium::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

float Terrarium::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void Terrarium::StartAdc()
{
    seed.adc.Start();
}

void Terrarium::StopAdc()
{
    seed.adc.Stop();
}


void Terrarium::ProcessAnalogControls()
{
    for(size_t i = 0; i < POT_LAST; i++)
    {
        pots[i].Process();
    }
}

float Terrarium::GetPotValue(Pot p)
{
    size_t idx;
    idx = p < POT_LAST ? p : POT_1;
    return pots[idx].Value();
}

void Terrarium::ProcessDigitalControls()
{
    for(size_t i = 0; i < FTSW_LAST; i++)
    {
        footswitches[i].Debounce();
    }
    for(size_t i = 0; i < SW_LAST; i++)
    {
        switches[i].Debounce();
    }
}


void Terrarium::ClearLeds()
{
    for(size_t i = 0; i < FOOTSWITCH_LED_LAST; i++)
    {
        SetLed(static_cast<FtswLed>(i), 0.0f);
    }
}

void Terrarium::UpdateLeds()
{
    for(size_t i = 0; i < FOOTSWITCH_LED_LAST; i++)
    {
        leds[i].Update();
    }
}


void Terrarium::SetLed(FtswLed idx, float bright)
{
    leds[idx].set(bright);
}


void Terrarium::SetHidUpdateRates()
{
    for(size_t i = 0; i < KNOB_LAST; i++)
    {
        knob[i].SetSampleRate(AudioCallbackRate());
    }
    for(size_t i = 0; i < FOOTSWITCH_LED_LAST; i++)
    {
        footswitch_led[i].SetSampleRate(AudioCallbackRate());
    }
    expression.SetSampleRate(AudioCallbackRate());
}

void Terrarium::InitSwitches()
{
    constexpr Pin sw_pin_numbers[SW_LAST] = {
        SW_1_PIN,
        SW_2_PIN,
        SW_3_PIN,
        SW_4_PIN
    };
    constexpr Pin ftsw_pin_numbers[FTSW_LAST] = {
        FTSW_1_PIN,
        FTSW_2_PIN
    };
    for(size_t i = 0; i < SW_LAST; i++)
    {
        switches[i].Init(sw_pin_numbers[i]);
    }
    for(size_t i = 0; i < FTSW_LAST; i++)
    {
        footswitches[i].Init(ftsw_pin_numbers[i]);
    }

}

void Terrarium::InitLeds()
{
    constexpr Pin led_pin_numbers[LED_LAST] = {
        LED_1_PIN,
        LED_2_PIN,
    };
    for(size_t i = 0; i < SW_LAST; i++)
    {
        leds[i].Init(led_pin_numbers[i]);
    }
}

void Terrarium::InitAnalogControls()
{
    AdcChannelConfig cfg[POT_LAST];

    cfg[POT_1].InitSingle(POT_1_PIN);
    cfg[POT_2].InitSingle(POT_2_PIN);
    cfg[POT_3].InitSingle(POT_3_PIN);
    cfg[POT_4].InitSingle(POT_4_PIN);
    cfg[POT_5].InitSingle(POT_5_PIN);
    cfg[POT_6].InitSingle(POT_6_PIN);

    seed.adc.Init(cfg, POT_LAST);
    // Make an array of pointers to the knob.
    for(int i = 0; i < POT_LAST; i++)
    {
        pot[i].Init(seed.adc.GetPtr(i), AudioCallbackRate());
    }
}
