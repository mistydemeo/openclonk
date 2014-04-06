/** 
	Playground
	Author: Maikel
	
	Reward for completing the tutorials, a small playground where the player can test all ingame objects.
*/


protected func Initialize()
{
	// Tutorial goal.
	var goal = CreateObject(Goal_Tutorial);
	goal.Name = "$MsgGoalName$";
	goal.Description = "$MsgGoalDescription$";
	return;
}


/*-- Player Handling --*/

protected func InitializePlayer(int plr)
{
	// Position player's clonk.
	var clonk = GetCrew(plr, 0);
	clonk->SetPosition(624, 526);
	var effect = AddEffect("ClonkRestore", clonk, 100, 10);
	effect.to_x = 624;
	effect.to_y = 526;
	
	// Standard player zoom for tutorials.
	SetPlayerViewLock(plr, true);
	SetPlayerZoomByViewRange(plr, 400, nil, PLRZOOM_Direct);
	SetPlayerZoomByViewRange(plr, 1200, nil, PLRZOOM_LimitMax);

	// Create tutorial guide, add messages, show first.
	var guide = CreateObject(TutorialGuide, 0, 0 , plr);
	guide->AddGuideMessage("$MsgPlaygroundWelcome$");
	guide->ShowGuideMessage(0);
	AddEffect("RemoveGuide", clonk, 100, 36 * 10);
	
	return;
}

global func FxRemoveGuideTimer(object target, proplist effect, int time)
{
	return -1;
}

global func FxRemoveGuideStop(object target, effect, int reason, bool  temporary)
{
	if (temporary)
		return;
	
	var guide = FindObject(Find_ID(TutorialGuide));
	if (guide)
	{
		guide->HideGuide();
		guide->RemoveObject();	
	}
}


/*-- Clonk Restoring --*/

global func FxClonkRestoreTimer(object target, proplist effect, int time)
{
	return 1;
}

// Relaunches the clonk, from death or removal.
global func FxClonkRestoreStop(object target, effect, int reason, bool  temporary)
{
	if (reason == 3 || reason == 4)
	{
		var restorer = CreateObject(ObjectRestorer, 0, 0, NO_OWNER);
		var x = BoundBy(target->GetX(), 0, LandscapeWidth());
		var y = BoundBy(target->GetY(), 0, LandscapeHeight());
		restorer->SetPosition(x, y);
		var to_x = effect.to_x;
		var to_y = effect.to_y;
		// Respawn new clonk.
		var plr = target->GetOwner();
		var clonk = CreateObject(Clonk, 0, 0, plr);
		clonk->GrabObjectInfo(target);
		SetCursor(plr, clonk);
		clonk->DoEnergy(100000);
		restorer->SetRestoreObject(clonk, nil, to_x, to_y, 0, "ClonkRestore");
	}
	return 1;
}