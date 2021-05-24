#include <pspkernel.h>
#include <pspgu.h>

int running();
int setupCallbacks(void);

PSP_MODULE_INFO("Cube Sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

static int exitRequest = 0;

void init_scegu(void);
void render_scegu(void);

int exitCallback(int arg1, int arg2, void *common)
{
	exitRequest = 1;
	return 0;
}

int callbackThread(SceSize args, void *argp)
{
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int setupCallbacks(void)
{
	int thid = 0;
	thid = sceKernelCreateThread
	(
		/* .threadName = */ "update_thread",
		/* .callbackProc = */ callbackThread,
		0x11,
		0xFA0,
		0,
		0
	);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}

int
main(int argc, char *argv[])
{
	setupCallbacks();
	init_scegu();
	while(!exitRequest)
	{
		render_scegu();
	}
	sceGuTerm();
	sceKernelExitGame();
	return 0;
}
