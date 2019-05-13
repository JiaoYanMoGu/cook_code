#include <thread>
#include <random>


static thread_local std::mt19937* PRNG = nullptr;

static const unsigned kRandomPRNGSeed = std::numeric_limits<unsigned>::max();

void SetPRNGSeed(unsigned seed = kRandomPRNGSeed)
{
// Overwrite existing PRNG
  if (PRNG != nullptr) {
    delete PRNG;
  }

  if (seed == kRandomPRNGSeed) {
    seed = static_cast<unsigned>(
        std::chrono::system_clock::now().time_since_epoch().count());
  }

  PRNG = new std::mt19937(seed);
  srand(seed);
}

template <typename T>
T RandomInteger(const T min, const T max);


template <typename T>
T RandomReal(const T min, const T max);

template <typename T>
T RandomGaussian(const T mean, const T stddev);

/////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////

template <typename T>
T RandomInteger(const T min, const T max) {
  if (PRNG == nullptr) {
    SetPRNGSeed();
  }

  std::uniform_int_distribution<T> distribution(min, max);

  return distribution(*PRNG);
}

template <typename T>
T RandomReal(const T min, const T max) {
  if (PRNG == nullptr) {
    SetPRNGSeed();
  }

  std::uniform_real_distribution<T> distribution(min, max);

  return distribution(*PRNG);
}


template <typename T>
T RandomGaussian(const T mean, const T stddev) {
  if (PRNG == nullptr) {
    SetPRNGSeed();
  }

  std::normal_distribution<T> distribution(mean, stddev);
  return distribution(*PRNG);
}
