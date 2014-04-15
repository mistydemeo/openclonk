// Artificial intelligence for the wipf.

#appendto Wipf

protected func Initialize()
{
	_inherited(...);
	
	RemoveEffect("Activity", this);
	
	AddEffect("TutorialWipf", this, 1, 5, this);
	return;
}

protected func FxTutorialWipfStart(object target, proplist effect, int temp)
{
	if (temp)
		return;
		
	effect.Sequence = "WaitABit";	
	return FX_OK;
}

protected func FxTutorialWipfTimer(object target, proplist effect, int time)
{

	return 1;
}