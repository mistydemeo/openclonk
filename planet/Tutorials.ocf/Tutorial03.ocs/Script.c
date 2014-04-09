/** 
	Tutorial 03: 
	Author: Maikel
	
	Repair the wipf statue: explains contents menu, dynamite, carry heavy materials, chopping, foundry and construction site.

	Following controls and concepts are explained:
	 * Contents menu: foundry and chest
	 * Production menu: foundry
	 * Using items: dynamite
	 * Heavy materials: 
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
	InitAI();
	
	// Dialogue options -> repeat round.
	SetNextMission("Tutorials.ocf\\Tutorial03.ocs", "$MsgRepeatRound$", "$MsgRepeatRoundDesc$");
	return;
}

// Gamecall from goals, set next mission.
protected func OnGoalsFulfilled()
{
	// Dialogue options -> next round.
	//SetNextMission("Tutorials.ocf\\Tutorial04.ocs", "$MsgNextTutorial$", "$MsgNextTutorialDesc$");
	// Normal scenario ending by goal library.
	return false;
}

private func InitVillageEntrance()
{
	// A wooden cabin ruin.
	var cabin = CreateObject(Ruin_WoodenCabin, 56, 384);
	cabin->AddScorch(-30, 15, 45, 32, 1200);
	cabin->AddScorch(15, 0, -45, 50, 1500);
	
	// Create a small forest.
	CreateObject(Tree_Coniferous, 128, 384);
	CreateObject(Tree_Coniferous, 148, 384);
	CreateObject(Tree_Coniferous, 166, 384);
	CreateObject(Tree_Coniferous, 188, 384);
	
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
	
	// A lorry with mining equipment.
	var lorry = CreateObject(Lorry, 212, 640);
	lorry->CreateContents(Dynamite, 3);

	return;
}

private func InitVillageMain()
{
	// Wipf statue construction site.
	var site = CreateObject(ConstructionSite, 372, 352);
	site->SetPosition(372, 352);
	site->SetProperty("MeshTransformation", Trans_Mul(Trans_Rotate(RandomX(-30, 30), 0, 1, 0), Trans_Rotate(RandomX(-10, 10), 1, 0, 0)));
	site->Set(WipfStatue);
	
	// Elevator, and grain production on the right.
	var elevator = CreateObject(Elevator, 601, 392);
	elevator->CreateShaft(264);
	CreateObject(Kitchen, 664, 392);
	var mill = CreateObject(Ruin_Windmill, 816, 392);
	mill->AddScorch(-15, 42, 90, 50, 1200);
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
	// A wooden cabin ruin.
	var cabin = CreateObject(Ruin_WoodenCabin, 574, 568);
	cabin->AddScorch(-20, 12, 45, 50, 1500);
	cabin->AddScorch(15, 10, -45, 50, 1500);
	
	// Chemical lab ruin and flagpole on the left of the elevator.
	var lab = CreateObject(Ruin_ChemicalLab, 484, 664);
	lab->AddScorch(-12, 18, 130, 80, 1300);
	CreateObject(Flagpole, 528, 664);
	
	// Steam engine for power supply below.
	var engine = CreateObject(SteamEngine, 780, 752);
	engine->CreateContents(Chunk_Coal, 5);
	
	// lorry with materials down below, the materials are only added in tutorial 4.
	CreateObject(Lorry, 954, 670);	
	return;
}

private func InitAnimals()
{
	// The wipf as your friend, controlled by AI.
	CreateObject(Wipf, 116, 384);
	
	// Some butterflies as atmosphere.
	for (var i = 0; i < 25; i++)
		PlaceAnimal(Butterfly);
	return;
}

// Initializes the AI: which is all defined in System.ocg
private func InitAI()
{
	// There will be a clonk extinguishing the burning places.
	//CreateFireman(280, 384);

	return;
}

/*-- Player Handling --*/

protected func InitializePlayer(int plr)
{
	// Position player's clonk.
	var clonk = GetCrew(plr, 0);
	clonk->SetPosition(104, 374);
	clonk->CreateContents(Shovel);
	var effect = AddEffect("ClonkRestore", clonk, 100, 10);
	effect.to_x = 40;
	effect.to_y = 374;
	
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
	AddEffect("TutorialFoundWipfStatue", nil, 100, 5);
	return;
}

/*-- Intro, Tutorial Goal & Outro --*/

global func FxTrackGoalTimer(object target, proplist effect, int time)
{
	if (FindObject(Find_ID(WipfStatue)))
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
	guide->ShowGuideMessage(12);
		
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

global func FxTutorialFoundWipfStatueTimer()
{
	if (FindObject(Find_OCF(OCF_CrewMember), Find_InRect(320, 296, 96, 56)))
	{
		guide->AddGuideMessage("$MsgTutorialStatue$");
		guide->ShowGuideMessage(1);
		AddEffect("TutorialFoundOreMine", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialFoundOreMineTimer()
{
	if (FindObject(Find_OCF(OCF_CrewMember), Find_InRect(168, 576, 104, 72)))
	{
		guide->AddGuideMessage("$MsgTutorialDynamiteLorry$");
		guide->ShowGuideMessage(2);
		AddEffect("TutorialObtainedDynamite", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialObtainedDynamiteTimer()
{
	var clonk = FindObject(Find_OCF(OCF_CrewMember));
	if (clonk && FindObject(Find_ID(Dynamite), Find_Container(clonk)))
	{
		guide->AddGuideMessage("$MsgTutorialBlastOre$");
		guide->ShowGuideMessage(3);
		AddEffect("TutorialBlastedOre", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialBlastedOreTimer()
{
	if (FindObject(Find_ID(Chunk_Ore), Find_InRect(240, 592, 100, 64)))
	{
		guide->AddGuideMessage("$MsgTutorialPickUpOre$");
		guide->ShowGuideMessage(4);
		AddEffect("TutorialObtainedOre", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialObtainedOreTimer()
{
	var clonk = FindObject(Find_OCF(OCF_CrewMember));
	if (clonk && FindObject(Find_ID(Chunk_Ore), Find_Container(clonk)))
	{
		guide->AddGuideMessage("$MsgTutorialOreToFoundry$");
		guide->ShowGuideMessage(5);
		AddEffect("TutorialOreInFoundry", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialOreInFoundryTimer()
{
	var foundry = FindObject(Find_ID(Foundry));
	if (foundry && FindObject(Find_ID(Chunk_Ore), Find_Container(foundry)))
	{
		guide->AddGuideMessage("$MsgTutorialCoalToFoundry$");
		guide->ShowGuideMessage(6);
		AddEffect("TutorialCoalInFoundry", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialCoalInFoundryTimer()
{
	var foundry = FindObject(Find_ID(Foundry));
	if (foundry && FindObject(Find_ID(Chunk_Coal), Find_Container(foundry)))
	{
		guide->AddGuideMessage("$MsgTutorialProduceMetal$");
		guide->ShowGuideMessage(7);
		AddEffect("TutorialMetalInQueue", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialMetalInQueueTimer()
{
	var foundry = FindObject(Find_ID(Foundry));
	if (!foundry)
		return 1;
		
	var effect = GetEffect("ProcessProduction", foundry);
	if (!effect)
		return 1;

	if (effect.Product == Chunk_Metal)
	{
		guide->AddGuideMessage("$MsgTutorialFindAxe$");
		guide->ShowGuideMessage(8);
		AddEffect("TutorialFoundAxe", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialFoundAxeTimer()
{
	var clonk = FindObject(Find_OCF(OCF_CrewMember));
	if (clonk && FindObject(Find_ID(Axe), Find_Container(clonk)))
	{
		guide->AddGuideMessage("$MsgTutorialDropMetal$");
		guide->ShowGuideMessage(9);
		AddEffect("TutorialDroppedMetal", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialDroppedMetalTimer()
{
	var site = FindObject(Find_ID(ConstructionSite));
	if (site && FindObject(Find_Or(Find_ID(Chunk_Metal), Find_ID(GoldBar)), Find_Container(site)))
	{
		guide->AddGuideMessage("$MsgTutorialChopTree$");
		guide->ShowGuideMessage(10);
		AddEffect("TutorialFellTree", nil, 100, 5);
		return -1;
	}
	return 1;
}

global func FxTutorialFellTreeTimer()
{
	if (ObjectCount(Find_ID(Chunk_Wood), Find_InRect(100, 300, 200, 200)) >= 2)
	{
		guide->AddGuideMessage("$MsgTutorialConstructStatue$");
		guide->ShowGuideMessage(11);
		return -1;
	}
	return 1;
}

protected func OnGuideMessageShown(int plr, int index)
{
	// Show the clonks friend: the wipf.	
	if (index == 0)
		TutArrowShowTarget(FindObject(Find_ID(Wipf))); 
	// Show the statue and the mine entrance.
	if (index == 1)
	{
		TutArrowShowTarget(FindObject(Find_ID(ConstructionSite))); 
		TutArrowShowPos(428, 392, 225);
		TutArrowShowPos(368, 516, 225);
	}
	// Show the lorry with dynamite.
	if (index == 2)
		TutArrowShowTarget(FindObject(Find_ID(Lorry), Sort_Distance(212, 632))); 
	// Show the ore to blast.
	if (index == 3)
		TutArrowShowPos(304, 628, 90);
	// Show the closest chunks of ore.
	if (index == 4)
	{
		var clonk = FindObject(Find_OCF(OCF_CrewMember));
		var ore = FindObject(Find_ID(Chunk_Ore), Sort_Distance(clonk->GetX(), clonk->GetY()));
		TutArrowShowTarget(ore);
	}
	// Show the foundry to drop the ore.
	if (index == 5)
		TutArrowShowTarget(FindObject(Find_ID(Foundry))); 
	// Show the coal to dig out.
	if (index == 6)
		TutArrowShowPos(136, 628, 270);
	// Show the foundry for producing the metal.
	if (index == 7)
		TutArrowShowTarget(FindObject(Find_ID(Foundry))); 
	// Show way to blast with dynamite and show the axe.
	if (index == 8)
	{
		TutArrowShowPos(292, 512, 315);
		TutArrowShowTarget(FindObject(Find_ID(Axe))); 
	}
	// Show way up to the forest, the piece of metal and the wipf site.
	if (index == 9)
	{
		TutArrowShowPos(408, 476, 45);
		TutArrowShowTarget(FindObject(Find_ID(Metal), Find_InRect(400, 440, 104, 64)));
		TutArrowShowTarget(FindObject(Find_ID(ConstructionSite)));
	}
	// Show the forest.
	if (index == 10)
	{
		TutArrowShowPos(184, 320, 225);
	}
	// Show the logs and the wipf statue.
	if (index == 11)
	{
		TutArrowShowTarget(FindObject(Find_ID(ConstructionSite))); 
		TutArrowShowTarget(FindObject(Find_ID(Chunk_Wood), Find_InRect(100, 250, 200, 200))); 
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