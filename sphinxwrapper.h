#ifndef SPHINXWRAPPER_H
#define SPHINXWRAPPER_H
#include <pocketsphinx/pocketsphinx.h>
#include <string>

class SphinxWrapper
{
public:
  SphinxWrapper();
  ~SphinxWrapper();

  /*!
   * \brief recognize speech in the audio file
   * \param filename name of the audio file
   * \return std::string with recognized text
   */
  std::string recognize(const std::string & filename);

private:
    ps_decoder_t *ps;
    cmd_ln_t *config;
    FILE *fh;
    char const *hyp, *uttid;
          int16 buf[512];
    int rv;
    int32 score;
};

#endif // SPHINXWRAPPER_H
