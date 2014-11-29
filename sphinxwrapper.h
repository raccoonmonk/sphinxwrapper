#ifndef SPHINXWRAPPER_H
#define SPHINXWRAPPER_H
#include <pocketsphinx/pocketsphinx.h>
#include <string>

class SphinxWrapper
{
public:
  SphinxWrapper();

  /*!
   * \brief recognize speech from an audio file
   * \param filename name of the audio file
   * \return std::string with recognized text
   */
  std::string recognize(const std::string & filename);

  /*!
   * \brief recognize speech from a block of memory
   * \param buf pointer to a data block
   * \param size size of dlock in bytes
   * \return std::string with recognized text
   */
  std::string recognize(const char * buf, int size);

private:
  ps_decoder_t *ps;
  cmd_ln_t *config;
  FILE *fh;
  char const *hyp, *uttid;
  int rv;
  int32 score;
};

#endif // SPHINXWRAPPER_H
