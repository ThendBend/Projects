IMPORTANT NOTE: If you want to use this library, make sure you include "ReqPicture.h" before including anything else for example -
#include "ReqPicture.h"
#include <iostream>
#include <windows.h>

This uses the SSPI Schannel C/C++ libraries which connects to https webservers on port 443 using RSA SSL/TLS security contexts. It will fetch images from google and dump the binary image to a specified file. You can play around with it and make it work how you want it to. It works with no errors on C++. You can comment on this respiratory if you need help with something.


I'm still working on this, it fetches a bit slow, but Ill try threading it later.

Look at the Source.cpp for an example of how to fetch using the ReqPicture header file.

If you want to go even deeper then look at the FetchPicture() function inside ReqPicture.h

You can then slowly go deeper from there, by looking at the ReqPicture class functions which FetchPicture() calls.

NOTE: You can also change the cnt and cnt2 variables in the FetchPicture() function in ReqPicture.h, increasing/decreasing it for more/less tries.

You can now specify if you want to use ipv6 or ipv4, by adding a third argument in the FetchPicture() function. PF_INET for ipv4 or PF_INET6 for ipv6. If left empty then ipv4 is chosen by default.

New feature: You can now leave the function FetchPicture() with empty arguments, to be prompted on the console to enter the information required, instead of hard coding it.
