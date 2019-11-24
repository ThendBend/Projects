This uses the SSPI Schannel C/C++ libraries which connects to https webservers on port 443 using RSA SSL/TLS security contexts. It will fetch images from google and dump the binary image to a specified file. You can play around with it and make it work how you want it to. It works with no errors on C++. You can comment on this respiratory if you need help with something.


I'm still working on this, it fetches a bit slow, but Ill try threading it later.

Look at the Source.cpp for an example of how to fetch using the ReqPicture header file.

If you want to go even deeper then look at the FetchPicture() function inside ReqPicture.h

You can then slowly go deeper from there, by looking at the ReqPicture class functions which FetchPicture() calls.
