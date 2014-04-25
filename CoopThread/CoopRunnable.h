/**
 * CoopRunnable.h
 */

#ifndef COOPRUNNABLE_H
#define COOPRUNNABLE_H

template <class SUPER, class TARGET>
class CoopRunnable : public SUPER {
public:
	typedef CoopRunnable<SUPER, TARGET> MyType;
	typedef SUPER super;
	typedef TARGET target;
	typedef bool (TARGET::*TargetFunc)(MyType*);
protected:
	TARGET* const mTarget;
	const TargetFunc mTargetFunc;
	bool mIsAlive;
public:
	CoopRunnable(TARGET* inTarget, TargetFunc inTargetFunc)
		: super(), mTarget(inTarget), mTargetFunc(inTargetFunc)
			{}
	virtual ~CoopRunnable() {}
	virtual bool Run()
		{ return (mTarget->*mTargetFunc)(this) ? true : (mIsAlive = false, false); }
	TARGET* GetTarget() { return mTarget; }
	const TARGET* GetTarget() const { return mTarget; }
};

#endif
