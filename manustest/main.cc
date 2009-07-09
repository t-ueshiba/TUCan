/*
 *  $Id: main.cc,v 1.2 2009-07-09 04:58:26 ueshiba Exp $
 */
/*!
  \mainpage	manustest
  \anchor	manustest
  $B%-!<%\!<%I%$%s%?!<%U%'!<%9$K$h$k(B
  <a href="http://www.exactdynamics.nl/">Exact Dynamics$B<R(B</a>$B@=$N(BManus
  $B%^%K%T%e%l!<%?$N$?$a$N%3%s%H%m!<%k%W%m%0%i%`$G$"$k!%(B
  CAN(Control Area Network)$B%G%P%$%9$N$?$a$N%3%s%H%m!<%i%i%$%V%i%j(B:
  \ref libTUCan "libTUCan++"$B$rMxMQ$7$F$$$k!%(B
*/
#include <signal.h>
#include <unistd.h>
#include "TU/Can++.h"

void	init_kbhit();
void	term_kbhit();
int	kbhit();

//! $B%a%$%s4X?t(B
/*!
  \param argc	$B%3%^%s%IL>$r4^$s$@0z?t$N?t(B
  \param argv	$B0z?tJ8;zNs$NG[Ns(B
*/
int
main(int argc, char* argv[])
{
    using namespace	std;
    using namespace	TU;
    
    char*		dev = "/dev/can0";
    extern char*	optarg;
    
    for (int c; (c = getopt(argc, argv, "d:")) != EOF; )
	switch (c)
	{
	  case 'd':
	    dev = optarg;
	    break;
	}

    try
    {
      // Manus$B%^%K%T%e%l!<%?%*%V%8%'%/%H$N@8@.(B
	Manus		manus(dev);

	init_kbhit();

      // $B%a%$%s%k!<%W(B
	for (int c; (c = kbhit()) != 'x'; )
	{
	    Manus::Speed	speed;	// $BB.EY;XNaCM(B($B$9$Y$F(B0$B$K=i4|2=(B)

	    switch (c)
	    {
	      case '0':
		manus.stillMode();	// STILL$B%b!<%I$K0\9T(B
		continue;
	      case '4':
		manus.jointMode();	// JOINT$B%b!<%I$K0\9T(B
		continue;
	      case '5':
		manus.foldOut();	// $B%^%K%T%e%l!<%?$r3H$2$k(B
		continue;
	      case '6':
		manus.foldIn();		// $B%^%K%T%e%l!<%?$r@^$j>v$`(B
		continue;
	    
	      case 'q':
		manus.setBaseUp().tick();	// $BBf:B$r>e$2$k(B
		continue;
	      case 'a':
		manus.setBaseDown().tick();	// $BBf:B$r2<$2$k(B
		continue;

	      case 'w':
		speed[0] =  Manus::MAX_SPEED_JOINT_012;
		break;
	      case 's':
		speed[0] = -Manus::MAX_SPEED_JOINT_012;
		break;
	      case 'e':
		speed[1] =  Manus::MAX_SPEED_JOINT_012;
		break;
	      case 'd':
		speed[1] = -Manus::MAX_SPEED_JOINT_012;
		break;
	      case 'r':
		speed[2] =  Manus::MAX_SPEED_JOINT_012;
		break;
	      case 'f':
		speed[2] = -Manus::MAX_SPEED_JOINT_012;
		break;
	      case 't':
		speed[3] =  Manus::MAX_SPEED_JOINT_345;
		break;
	      case 'g':
		speed[3] = -Manus::MAX_SPEED_JOINT_345;
		break;
	      case 'y':
		speed[4] =  Manus::MAX_SPEED_JOINT_345;
		break;
	      case 'h':
		speed[4] = -Manus::MAX_SPEED_JOINT_345;
		break;
	      case 'u':
		speed[5] =  Manus::MAX_SPEED_JOINT_345;
		break;
	      case 'j':
		speed[5] = -Manus::MAX_SPEED_JOINT_345;
		break;
	      case 'i':
		speed[6] =  Manus::MAX_SPEED_JOINT_GRIP;
		break;
	      case 'k':
		speed[6] = -Manus::MAX_SPEED_JOINT_GRIP;
		break;
	    }
	    speed *= 0.2;	// $B:G9bB.EY$N(B1/5$B$K$9$k(B
	    
	  // $BB.EY$r@_Dj$7$F%k!<%W$r(B1$B2s$^$o$9!%(B
	    manus.setSpeed(speed).tick();

	    if (manus.status() != Manus::OK)
		cerr << Manus::message(manus.status()) << endl;
	    cerr << "  Position: " << manus.position();
	}
	
      // $B%W%m%0%i%`=*N;;~$K$O(BSTILL$B%b!<%I$K$9$k(B
	manus.stillMode();

	term_kbhit();
    }
    catch (exception& err)
    {
	cerr << err.what() << endl;
	return 1;
    }
    
    return 0;
}
