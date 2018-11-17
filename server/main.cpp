#include "kptservice.h"

int main(int argc, char *argv[])
{
	KPTService svc{argc, argv};
	return svc.exec();
}
