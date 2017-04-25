#ifndef PROGRESSINDICATORINTERFACE_H
#define PROGRESSINDICATORINTERFACE_H

class ProgressIndicatorInterface
{
public:
	ProgressIndicatorInterface() {}
	virtual ~ProgressIndicatorInterface() {}
	
	virtual void updateProgress(int progress) = 0;
};

#endif
