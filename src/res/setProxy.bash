https_proxy=127.0.0.1:$$PORT$$
http_proxy=127.0.0.1:$$PORT$$
export https_proxy=127.0.0.1:$$PORT$$
export http_proxy=127.0.0.1:$$PORT$$
launchctl setenv https_proxy 127.0.0.1:$$PORT$$
launchctl setenv http_proxy 127.0.0.1:$$PORT$$
