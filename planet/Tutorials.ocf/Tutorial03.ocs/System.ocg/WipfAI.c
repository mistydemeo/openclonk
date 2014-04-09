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
	// Wait a bit to start walking.
	if (effect.Sequence == "WaitABit" && time >= 20)
	{
		SetCommand("MoveTo", nil, 352, 348);
		effect.Sequence = "WalkToStatue";
	}
	// Walk to statue and wait there.
	if (effect.Sequence == "WalkToStatue" && Inside(GetX(), 344, 360) && Inside(GetY(), 336, 352))
	{
		//SetCommand("None");
	}	
	return 1;
}