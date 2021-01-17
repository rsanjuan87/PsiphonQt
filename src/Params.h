#ifndef PARAMS_H
#define PARAMS_H

#include <QString>


class Params
{
public:
    Params() {}
    inline const static QString FROM_SECOND = " --from-second";//internal use
    inline const static QString SHOW_PAGE = "--show-";
    inline const static QString START_TUNNEL = "--start-tunnel";
    inline const static QString STOP_TUNNEL = "--stop-tunnel";
    inline const static QString TOGGLE_TUNNEL = "--toggle-tunnel";
    inline const static QString HELP = "--help";
    inline const static QString QUIT_APP = "--quit-app";
};

#endif // PARAMS_H
