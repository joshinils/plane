#include "Plane.h"

int main()
{
	Plane demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}