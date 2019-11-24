#include "ReqPicture.h"

int main()
{
	//You could specify a 3rd argument PF_INET for ipv4 or PF_INET6 for ipv6
	//if you want it to automatically choose then specify PF_UNSPEC in the 3rd argument(NOT recommended if you don't have ipv6 support)
	//If it is left empty then ipv4 is chosen by default

	if (!FetchPicture("funny pictures", "lester.jpg"))
		return -1;

	//You can also leave all arguments empty like this and be prompted in the console to type the information needed
	/*

	if(!FetchPicture())
		return -1;
	
	*/

	return 0;

}

/*atyp=csi&ei=m6nZXZjpJsm_lwSM6qNQ&s=jsa&jsi=s,et.ivg_o,n.qZ1Udb,cn.3&zx=1574546334944
*/