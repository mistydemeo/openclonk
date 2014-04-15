/** 
	Tutorial 04: 
	Author: Maikel
	
	Restore Power: restore the power chain to the elevator and move the lorry to the armory.

	Following controls and concepts are explained:
	 * Construction: hammer
	 * Interaction: elevator and lorry
	 * Production: bow & sword
*/


static guide; // guide object.

protected func Initialize()
{
	// Tutorial goal.
	var goal = CreateObject(Goal_Tutorial);
	goal.Name = "$MsgGoalName$";
	goal.Description = "$MsgGoalDescription$";
	
	// Place objects in different sections.
	InitVillageEntrance();
	InitVillageMain();
	InitOreMine();
	InitVillageUnderground();
	InitAnimals();
		
	// Dialogue options -> repeat round.
	SetNextMission("Tutorials.ocf\\Tutorial04.ocs", "$MsgRepeatRound$", "$MsgRepeatRoundDesc$");
	return;
}

// Gamecall from goals, set next mission.
protected func OnGoalsFulfilled()
{
	// Dialogue options -> next round.
	//SetNextMission("Tutorials.ocf\\Tutorial05.ocs", "$MsgNextTutorial$", "$MsgNextTutorialDesc$");
	// Normal scenario ending by goal library.
	return false;
}

private func InitVillageEntrance()
{
	// Cabin has been fixed.
	CreateObject(WoodenCabin, 56, 384);
	
	// Create a small forest.
	CreateObject(Tree_Coniferous, 128, 384);
	CreateObject(Tree_Coniferous, 148, 384);
	CreateObject(Tree_Coniferous, 166, 384);
	CreateObject(Tree_Coniferous, 188, 384);
	CreateObject(Chunk_Wood, 188, 380);
	
	// Wind generator and sawmill for wood production.
	CreateObject(Sawmill, 258, 384);
	CreateObject(WindGenerator, 224, 384);
	
	// A hole where the axe hides.
	CreateObject(Axe, 159, 474);

	return;
}

private func InitOreMine()
{
	// A foundry halfway down.
	CreateObject(Foundry, 456, 488);
	CreateObject(Chunk_Metal, 442, 486);
	
	// A lorry with mining equipment.
	var lorry = CreateObject(Lorry, 212, 640);
	lorry->CreateContents(Dynamite, 3);

	return;
}

private func InitVillageMain()
{
	// Wipf statue is now completed.
	var statue = CreateObject(WipfStatue, 372, 352);
	statue->SetClrModulation(RGB(30, 50, 255));
	
	// Elevator, and grain production on the right.
	var elevator = CreateObject(Elevator, 601, 392);
	elevator->CreateShaft(264);
	CreateObject(Kitchen, 664, 392);
	CreateObject(Windmill, 816, 392);
	Wheat->Place(40 + Random(10), Rectangle(704, 352, 80, 64));
	CreateObject(StrawMan, 714, 395);
	CreateObject(StrawMan, 762, 395);
	
	// Stone door to protect the village.
	var door = CreateObject(StoneDoor, 1004, 376);
	door->SetAutoControl();
	
	// Tools and armory down below.	
	CreateObject(ToolsWorkshop, 698, 504);
	CreateObject(Armory, 772, 504);
	CreateObject(Flagpole, 828, 496);
	return;
}

private func InitVillageUnderground()
{
	// Cabin has been fixed.
	CreateObject(WoodenCabin, 574, 568);
	
	// Chemical lab and flagpole on the left of the elevator.
	CreateObject(ChemicalLab, 484, 664);
	CreateObject(Flagpole, 528, 664);
	
	// Steam engine for power supply below.
	var engine = CreateObject(SteamEngine, 780, 752);
	engine->CreateContents(Chunk_Coal, 5);
	
	// lorry with materials down below.
	var lorry = CreateObject(Lorry, 954, 670);
	lorry->CreateContents(Chunk_Wood, 8);
	lorry->CreateContents(Chunk_Metal, 4);
	lorry->CreateContents(Loam, 2);
	
	return;
}

private func InitAnimals()
{
	// The wipf as your friend, controlled by AI.
	CreateObject(Wipf, 352, 352);
	
	// Some butterflies as atmosphere.
	for (var i = 0; i < 25; i++)
		PlaceAnimal(Butterfly);
	return;
}	

/*-- Player Handling --*/

protected func InitializePlayer(int plr)
{
	// Position player's clonk.
	var clonk = GetCrew(plr, 0);
	clonk->SetPosition(396, 342);
	clonk->CreateContents(Shovel);
	clonk->CreateContents(Axe);
	clonk->CreateContents(Hammer);
	var effect = AddEffect("ClonkRestore", clonk, 100, 10);
	effect.to_x = 396;
	effect.to_y = 342;
	
	// Add knowledge to build a flagpole and construct sword and bow.
	SetPlrKnowledge(plr, Flagpole);
	SetPlrKnowledge(plr, Sword);
	SetPlrKnowledge(plr, Bow);
	
	// Claim ownership of structures.
	for (var structure in FindObjects(Find_Or(Find_Category(C4D_Structure), Find_Func("IsFlagpole"))))
		structure->SetOwner(plr);
	
	// Add an effect to the clonk to track the goal.
	AddEffect("TrackGoal", clonk, 100, 2);

	// Standard player zoom for tutorials.
	SetPlayerViewLock(plr, true);
	SetPlayerZoomByViewRange(plr, 400, nil, PLRZOOM_Direct);

	// Create tutorial guide, add messages, show first.
	guide = CreateObject(TutorialGuide, 0, 0, plr);
	guide->AddGuideMessage("$MsgTutorialIntro$");
	guide->ShowGuideMessage(0);
	AddEffect("TutorialAtConstructionSite", nil, 100, 5);
	return;
}

/*-- Intro, Tutorial Goal & Outro --*/

global func FxTrackGoalTimer(object target, proplist effect, int time)
{
	var armory = FindObject(Find_ID(Armory));
	if (!armory)
		return 1;	
	var sword = FindObject(Find_ID(Sword), Find_Container(armory));
	var bow = FindObject(Find_ID(Bow), Find_Container(armory));
	
	if (sword && bow)
	{
		AddEffect("GoalOutro", target, 100, 5);
		return -1;	
	}
	return 1;
}

global func FxGoalOutroStart(object target, proplist effect, int temp)
{
	if (temp)
		return;
		
	// Show guide message congratulating.
	guide->AddGuideMessage("$MsgTutorialCompleted$");
	guide->ShowGuideMessage(7);
		
	return 1;
}

global func FxGoalOutroTimer(object target, proplist effect, int time)
{
	if (time >= 54)
	{
		var goal = FindObject(Find_ID(Goal_Tutorial));
		goal->Fulfill();
		return -1;
	}
	return 1;
}

global func FxGoalOutroStop(object target, proplist effect, int reason, bool temp)
{
	if (temp) 
		return;
}

/*-- Guide Messages --*/

global func FxTutorialAtConstructionSiteTimer()
{
	if (FindObject(Find_OCF(OCF_CrewMember), Find_InRect(484, 344, 24, 32)))
	{
		guide->AddGuideMessage("$MsgTutorialConstruct$");
		guide->ShowGuideMessage(1);
		AddEffect("TutorialPlacedConstruction", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialPlacedConstructionTimer()
{
	if (FindObject(Find_ID(ConstructionSite), Find_InRect(484, 344, 24, 32)))
	{
		guide->AddGuideMessage("$MsgTutorialMaterials$");
		guide->ShowGuideMessage(2);
		AddEffect("TutorialConstructionDone", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialConstructionDoneTimer()
{
	if (FindObject(Find_ID(Flagpole), Find_InRect(484, 344, 24, 32)))
	{
		guide->AddGuideMessage("$MsgTutorialElevator$");
		guide->ShowGuideMessage(3);
		AddEffect("TutorialDroveElevatorDown", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialDroveElevatorDownTimer()
{
	if (FindObject(Find_ID(ElevatorCase), Find_InRect(604, 624, 32, 32)))
	{
		guide->AddGuideMessage("$MsgTutorialBringLorry$");
		guide->ShowGuideMessage(4);
		AddEffect("TutorialPushedLorry", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialPushedLorryTimer()
{
	if (FindObject(Find_ID(Lorry), Find_InRect(604, 624, 32, 32)))
	{
		guide->AddGuideMessage("$MsgTutorialLorryOnElevator$");
		guide->ShowGuideMessage(5);
		AddEffect("TutorialLorryAtArmory", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialLorryAtArmoryTimer()
{
	if (FindObject(Find_ID(Lorry), Find_InRect(736, 464, 64, 40)))
	{
		guide->AddGuideMessage("$MsgTutorialProduceWeapons$");
		guide->ShowGuideMessage(6);
		return -1;
	}
	return 1;
}

protected func OnGuideMessageShown(int plr, int index)
{
	// Show the construction location.	
	if (index == 0)
	{
		TutArrowShowPos(496, 368, 135);
		TutArrowShowPos(496, 368, 225);
	}
	// Show the metal and the forest.
	if (index == 2)
	{
		TutArrowShowTarget(FindObject(Find_ID(Metal), Find_InRect(400, 440, 104, 64)));
		TutArrowShowPos(184, 320, 225);
	}
	// Show the elevator case.
	if (index == 3)
	{
		TutArrowShowTarget(FindObject(Find_ID(ElevatorCase)));
	}
	// Show way to lorry and lorry itself.
	if (index == 4)
	{
		TutArrowShowPos(696, 600, 90);
		TutArrowShowTarget(FindObject(Find_ID(Lorry), Find_InRect(560, 520, 464, 248)));
	}
	// Show way to the armory.
	if (index == 5)
	{
		TutArrowShowTarget(FindObject(Find_ID(Armory)));
	}

	return;
}

protected func OnGuideMessageRemoved(int plr, int index)
{
	TutArrowClear();
	return;
}


/*-- Clonk restoring --*/


global func FxClonkRestoreTimer(object target, proplist effect, int time)
{
	// Respawn clonk to new location if reached certain position.
	if (FindObject(Find_OCF(OCF_CrewMember), Find_InRect(656, 432, 40, 48)))
	{
		effect.to_x = 680;
		effect.to_y = 470;             
	}
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