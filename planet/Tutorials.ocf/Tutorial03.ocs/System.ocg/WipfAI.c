// Artificial intelligence for the wipf.

#appendto Wipf

public func EnableTutorialControl(bool friend)
{
	RemoveEffect("IntActivity", this);
	var effect = AddEffect("TutorialWipf", this, 1, 5, this);
	if (!friend)
		effect.sequence = "StayAtGrainField";
	return;
}

public func DisableTutorialControl()
{
	RemoveEffect("TutorialWipf", this);
	AddEffect("IntActivity", this, 1, 10, this);
	return;
}

protected func FxTutorialWipfStart(object target, proplist effect, int temp)
{
	if (temp)
		return FX_OK;
	effect.sequence = "WaitABit";
	this.Collectible = false;	
	return FX_OK;
}

protected func FxTutorialWipfTimer(object target, proplist effect, int time)
{
	// Wait a bit to start walking.
	if (effect.sequence == "WaitABit" && time >= 20)
	{
		SetAction("Walk");
		SetCommand("MoveTo", nil, 700, 388);
		effect.sequence = "WalkToGrainField";
	}
	// Walk to statue and wait there.
	if (effect.sequence == "WalkToGrainField" && Inside(GetX(), 680, 720) && Inside(GetY(), 340, 420))
	{
		effect.sequence = "StayAtGrainField";
	}
	if (effect.sequence == "StayAtGrainField")
	{
		// Don't do anything if contained.
		if (Contained())
			return FX_OK;
		if (IsWalking()) 
		{
			if ((GetDir() == DIR_Left || GetComDir() == COMD_Left) && GetX() < 656)
			{
				SetComDir(COMD_Right);
				return FX_OK;
			}
			if ((GetDir() == DIR_Right || GetComDir() == COMD_Right) && GetX() > 832)
			{
				SetComDir(COMD_Left);
				return FX_OK;
			}
			if (!Random(3))
			{
				SetComDir([COMD_Left, COMD_Right][Random(2)]);
				return FX_OK;
			
			}
			// Stop walking from time to time.
			if (!Random(3) && GetComDir() != COMD_Stop)
			{
				SetComDir(COMD_Stop);
				return FX_OK;
			}
			// Start standing when com dir is stop and speed is zero.
			if (GetComDir() == COMD_Stop && GetXDir() == 0)
			{
				SetAction("Stand");
				return FX_OK;
			}
			return FX_OK;
		}
		if (IsStanding())
		{
			if (!Random(10))
			{
				SetAction("Walk");
				SetComDir([COMD_Left, COMD_Right][Random(2)]);
				return FX_OK;
			}
			return FX_OK;
		}
	}	
	return FX_OK;
}

protected func FxTutorialWipfStop(object target, proplist effect, int reason, bool temp)
{
	if (temp)
		return FX_OK;
	this.Collectible = true;
	return FX_OK;
}