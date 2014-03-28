// Artificial intelligence for the wipf.

#appendto Wipf

local had_food = false;

protected func Initialize()
{
	_inherited(...);
	
	RemoveEffect("Activity", this);
	
	AddEffect("TutorialWipf", this, 1, 5, this);
	return;
}

public func HadFood() { return had_food; }

protected func FxTutorialWipfStart(object target, proplist effect, int temp)
{
	if (temp)
		return;
		
	effect.Sequence = "WaitForFood";	
	return FX_OK;
}

protected func FxTutorialWipfTimer(object target, proplist effect, int time)
{
	// Wait for some food to appear.
	var food = FindObject(Find_Func("NutritionalValue"), Find_Distance(16), Find_NoContainer());
	if (effect.Sequence == "WaitForFood" && food)
	{
		Collect(food, true);
		had_food = true;	
		SetCommand("Follow", FindObject(Find_OCF(OCF_CrewMember)));
		effect.Sequence = "WalkToBridge";
	}	
	// Follow clonk until burned bridge.
	if (effect.Sequence == "WalkToBridge" && Inside(GetX(), 760, 816) && Inside(GetY(), 496, 528))
	{
		SetCommand("MoveTo", nil, 992, 524);
		effect.Sequence = "MoveToSettlement";
	}
	return 1;
}