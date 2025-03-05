#include "AudioBuffer.h"
AudioBuffer* AudioBuffer::load(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return nullptr;

    // Lee la cabecera del fichero WAV
    char chunkID[4];
    file.read(chunkID, 4);
    if (std::string(chunkID, 4) != "RIFF") return nullptr;

    file.seekg(22);
    uint16_t channels;
    file.read(reinterpret_cast<char*>(&channels), 2);

    uint32_t sampleRate;
    file.read(reinterpret_cast<char*>(&sampleRate), 4);

    file.seekg(36);
    uint16_t bitsPerSample;
    file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

    // Encuentra el bloque "data"
    char dataHeader[4];
    uint32_t dataSize;
    do 
    {
        file.read(dataHeader, 4);
        file.read(reinterpret_cast<char*>(&dataSize), 4);
        if (std::string(dataHeader, 4) != "data")
            file.seekg(dataSize, std::ios::cur);
    } while (std::string(dataHeader, 4) != "data" && file);

   if (!file) return nullptr;

    // Lee los datos de audio
    std::vector<char> data(dataSize);
    file.read(data.data(), dataSize);

    // Genera el buffer
    ALuint alBuffer;
    alGenBuffers(1, &alBuffer);

    ALenum format = (channels == 1)
        ? (bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16)
        : (bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16);

    alBufferData(alBuffer, format, data.data(), dataSize, sampleRate);

    AudioBuffer* audioBuffer = new AudioBuffer();
    audioBuffer->buffer = alBuffer;
    return audioBuffer;
}

ALuint AudioBuffer::getAlBuffer() const
{
    return buffer;
}

AudioBuffer::~AudioBuffer()
{
    alDeleteBuffers(1, &buffer);
}
