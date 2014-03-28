// Artificial intelligence for the wipf and also the elevator.

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
		
	effect.Sequence = "Introduction";
	effect.StartMoving = false;	
	return FX_OK;
}

public func StartMoving()
{
	var effect = GetEffect("TutorialWipf", this, 0);
	effect.StartMoving = true;
}

protected func FxTutorialWipfTimer(object target, proplist effect, int time)
{
	// Wait for the introduction to finish and then move through the first hole.
	if (effect.Sequence == "Introduction" && effect.StartMoving)
	{
		SetCommand("MoveTo", nil, 344, 612);
		effect.Sequence = "WaitInFrontOfHole";
		FindObject(Find_ID(ElevatorCase))->SetMoveDirection(ElevatorCase_down, true, false);
	}	
	
	// Wait for the introduction to finish and then move through the first hole.
	if (effect.Sequence == "WaitInFrontOfHole" && FindObject(Find_OCF(OCF_CrewMember), Find_InRect(AbsX(256), AbsY(576), 80, 40)))
	{
		SetCommand("MoveTo", nil, 548, 604);
		effect.Sequence = "MoveThroughHole";
	}	
	
	// Wait after the first hole until the clonk arrives and move through the second.
	if (effect.Sequence == "MoveThroughHole" && FindObject(Find_OCF(OCF_CrewMember), Find_InRect(AbsX(424), AbsY(464), 64, 64)))
	{
		SetCommand("MoveTo", nil, 780, 604);
		AddCommand("MoveTo", nil, 676, 684);
		effect.Sequence = "MoveAcross";		
	}
	
	// Wait after the second hole until the clonk arrives and move through the third.
	if (effect.Sequence == "MoveAcross" && FindObject(Find_OCF(OCF_CrewMember), Find_InRect(AbsX(664), AbsY(560), 80, 48)))
	{
		SetCommand("MoveTo", nil, 890, 636);
		effect.Sequence = "MoveToLift";	
	}
	
	// Wait for and move up the elevator.
	if (effect.Sequence == "MoveToLift" && Inside(GetX(), 885, 895) && Inside(GetY(), 632, 640) && !GetCommand())
	{
		SetCommand("None");
		SetPosition(891, 634);
		effect.Sequence = "WaitOnLift";	
		Schedule(FindObject(Find_ID(ElevatorCase)), "SetMoveDirection(ElevatorCase_up, true, false)", 15);
		Schedule(FindObject(Find_ID(ElevatorCase)), "SetMoveDirection(ElevatorCase_down, true, false)", 17);
		Schedule(FindObject(Find_ID(ElevatorCase)), "SetMoveDirection(ElevatorCase_up, true, false)", 20);
	}
	
	// If lift is up again, move to cabin.
	if (effect.Sequence == "WaitOnLift" && Inside(GetX(), 885, 895) && Inside(GetY(), 320, 328))
	{
		SetCommand("MoveTo", nil, 992, 324);
		effect.Sequence = "MoveToCabin";	
	}

	return 1;
}