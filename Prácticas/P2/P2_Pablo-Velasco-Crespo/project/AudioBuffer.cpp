#include "AudioBuffer.h"
AudioBuffer* AudioBuffer::load(const char* filename)
{
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) return nullptr;

  // Leer cabecera RIFF
  char riffHeader[12];
  file.read(riffHeader, 12);
  if (std::string(riffHeader, 4) != "RIFF" ||
    std::string(riffHeader + 8, 4) != "WAVE") 
  {
    return nullptr;
  }

  // Buscar los chunks fmt y data
  uint16_t channels = 0;
  uint32_t sampleRate = 0;
  uint16_t bitsPerSample = 0;
  uint32_t dataSize = 0;
  std::vector<char> audioData;

  char chunkHeader[8];
  while (file.read(chunkHeader, 8))
  {
    std::string chunkId(chunkHeader, 4);
    uint32_t chunkSize;
    memcpy(&chunkSize, chunkHeader + 4, 4);

    if (chunkId == "fmt ") 
    {
      uint16_t formatType;
      file.read(reinterpret_cast<char*>(&formatType), 2);
      if (formatType != 1) return nullptr;

      file.read(reinterpret_cast<char*>(&channels), 2);
      file.read(reinterpret_cast<char*>(&sampleRate), 4);
      file.ignore(6);
      file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

      if (chunkSize > 16)
      {
        file.ignore(chunkSize - 16);
      }
    }
    else if (chunkId == "data") 
    {
      dataSize = chunkSize;
      audioData.resize(dataSize);
      file.read(audioData.data(), dataSize);
      break;
    }
    else
    {
      file.ignore(chunkSize);
    }
  }

  if (audioData.empty()) return nullptr;

  ALuint alBuffer;
  alGenBuffers(1, &alBuffer);

  ALenum format;
  if (channels == 1)
  {
    format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
  }
  else if (channels == 2)
  {
    format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
  }
  else 
  {
    alDeleteBuffers(1, &alBuffer);
    return nullptr;
  }

  alBufferData(alBuffer, format, audioData.data(), dataSize, sampleRate);

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
