/*-- 
	Tutorial 01: Wandering Wipf
	Author: Maikel
	
	First introduction to the world of OpenClonk: explains basic movement controls.
	
	Following controls and concepts are explained:
	 * Clonk HUD 
	 * Walking and jumping with WASD
	 * Scaling, wall jump and hangling
	 * Swimming, diving and breath 
--*/

static guide; // guide object

protected func Initialize()
{
	// Tutorial goal.
	var goal = CreateObject(Goal_Tutorial);
	goal.Name = "$MsgGoalName$";
	goal.Description = "$MsgGoalDescription$";
	CreateObject(Rule_NoPowerNeed);
	
	// Place objects in different sections.
	InitStartLake();
	InitCaveEntrance();
	InitHangleSection();
	InitCaveCenter();
	InitGoldMine();
	InitCaveExit();
	InitVegetation();
	InitAnimals();
	
	// Environment.
	CreateObject(Environment_Celestial);
	var time = CreateObject(Environment_Time);
	time->SetTime(18*60 + 30);
	time->SetCycleSpeed(0);

	// Dialogue options -> repeat round.
	SetNextMission("Tutorials.ocf\\Tutorial01.ocs", "$MsgRepeatRound$", "$MsgRepeatRoundDesc$");
	return;
}

// Gamecall from goals, set next mission.
protected func OnGoalsFulfilled()
{
	// Dialogue options -> next round.
	SetNextMission("Tutorials.ocf\\Tutorial02.ocs", "$MsgNextTutorial$", "$MsgNextTutorialDesc$");
	// Normal scenario ending by goal library.
	return false;
}

// Lake and start position.
private func InitStartLake()
{
	// Seaweed, fishes and shells in the lakes.
	Seaweed->Place(5, Rectangle(0, 600, 50, 100));
	Seaweed->Place(5, Rectangle(120, 600, 120, 120));
	Seaweed->Place(5, Rectangle(220, 400, 140, 50));
	
	// A shipwreck on the strand.
	
	// A few coconut trees.
	CreateObject(Tree_Coconut, 52, 620);
	CreateObject(Tree_Coconut, 79, 620);
	CreateObject(Tree_Coconut, 105, 620);
	
	// Ropebridge over the lake.
	var post1 = CreateObject(Ropebridge_Post, 144, 616);
	var post2 = CreateObject(Ropebridge_Post, 232, 616);
	post2->SetObjDrawTransform(-1000, 0, 0, 0, 1000);
	post2.Double->SetObjDrawTransform(-1000, 0, 0, 0, 1000);
	CreateObject(Ropebridge, 186, 616)->MakeBridge(post1, post2);
	
	// Small sink hole for the water to flow through, cover by a trunk.
	DrawMaterialQuad("Tunnel", 231, 631, 231, 633, 251, 615, 249, 615);
	var trunk = CreateObject(Trunk, 241, 644);
	trunk->SetR(45); trunk.Plane = 510; trunk->DoCon(-42);
	trunk.MeshTransformation = [-1000, 0, -17, 0, 0, 1000, 0, 0, 17, 0, -1000, 0];

	return;
}

// Cave entrance with waterfall.
private func InitCaveEntrance()
{
	// Waterfall from a leaking trunk.
	var trunk = CreateObject(Trunk, 324, 465);
	trunk->SetR(150); trunk.Plane = 510;
	trunk.MeshTransformation = [-70, 0, 998, 0, 0, 1000, 0, 0, -998, 0, -70, 0];
	var waterfall;
	waterfall = CreateWaterfall(325, 448, 2, "Water");
	waterfall->SetDirection(2, 0, 3, 6);
	waterfall = CreateWaterfall(338, 450, 8, "Water");
	waterfall->SetDirection(1, 0, 4, 8);
	CreateLiquidDrain(160, 648, 10);
	CreateLiquidDrain(184, 648, 10);
	CreateLiquidDrain(208, 648, 10);
	
	// Some vegetation below the waterfall.
	Fern->Place(2 + Random(2), Rectangle(300, 600, 60, 40));
	Mushroom->Place(4 + Random(2), Rectangle(300, 600, 60, 40));
	
	// Make the wipf hole a bit wider.
	DrawMaterialQuad("Tunnel", 360, 600, 410, 600, 410, 589, 360, 589);
	
	// Vegetation in wall jump hole.
	var rock = CreateObject(BigRock, 313, 525);
	rock.MeshTransformation = [-314, -543, 671, 0, 0, 1085, 878, 0, -974, 222, -275, 0];
	rock.Plane = 510;
	
	// Some vegetation on the walls.
	var rank = CreateObject(Rank, 313, 461);
	rank.MeshTransformation = [292, 0, 956, 0, 0, 1000, 0, 0, -956, 0, 292, 0];
	rank->SetR(80 + Random(20)); rank->DoCon(15);
	rank = CreateObject(Rank, 338, 433); 
	rank.MeshTransformation = [-999, 0, 52, 0, 0, 1000, 0, 0, -52, 0, -999, 0];
	rank->SetR(180); rank->DoCon(50); 
	rank = CreateObject(Rank, 390, 439); 
	rank.MeshTransformation = [588, 0, -809, 0, 0, 1000, 0, 0, 809, 0, 588, 0]; 	
	rank->SetR(165 + Random(30)); rank->DoCon(20); rank.Plane = 510;
	rank = CreateObject(Rank, 442, 463);
	rank.MeshTransformation = [391, 0, 921, 0, 0, 1000, 0, 0, -921, 0, 391, 0];
	rank->SetR(260 + Random(20)); rank->DoCon(10);
	
	return;
}

// Hangle section with large mushrooms and gap to cross.
private func InitHangleSection()
{
	// Some large mushrooms.
	var mushroom = CreateConstruction(LargeCaveMushroom, 489, 557, NO_OWNER, 100, false, false);
	mushroom.MeshTransformation = Trans_Scale(500, 500, 500); mushroom->SetR(240);
	mushroom = CreateConstruction(LargeCaveMushroom, 517, 606, NO_OWNER, 100, false, false);
	mushroom.MeshTransformation = Trans_Scale(550, 550, 550); 	
	mushroom = CreateConstruction(LargeCaveMushroom, 597, 670, NO_OWNER, 100, false, false);
	mushroom.MeshTransformation = Trans_Scale(600, 600, 600); 
	mushroom = CreateConstruction(LargeCaveMushroom, 642, 686, NO_OWNER, 100, false, false);
	mushroom.MeshTransformation = Trans_Scale(700, 700, 700); 
	
	// Some ferns and mushrooms.	
	Fern->Place(5 + Random(3), Rectangle(480, 560, 220, 140));
	Mushroom->Place(9 + Random(4), Rectangle(480, 560, 220, 140));
	
	// Some vegetation on the walls.
	var rank = CreateObject(Rank, 607, 566); 
	rank.MeshTransformation = [946, 0, 326, 0, 0, 1000, 0, 0, -326, 0, 946, 0];
	rank->SetR(165 + Random(30)); rank->DoCon(30); 
	rank = CreateObject(Rank, 637, 566); 
	rank.MeshTransformation = [956, 0, 292, 0, 0, 1000, 0, 0, -292, 0, 956, 0]; 	
	rank->SetR(165 + Random(30)); rank->DoCon(15); rank.Plane = 510;
	rank = CreateObject(Rank, 658, 566); 
	rank.MeshTransformation = [999, 0, -35, 0, 0, 1000, 0, 0, 35, 0, 999, 0]; 	
	rank->SetR(165 + Random(30)); rank->DoCon(5);
	
	// Make the diagonal wipf hole a bit wider.
	DrawMaterialQuad("Tunnel", 690, 671, 754, 607, 769, 607, 690, 686);
	
	// Wipf hole covered by trunk.	
	var trunk = CreateObject(Trunk, 822, 621);
	trunk.MeshTransformation = [821, 0, 795, 0, 0, 1145, 0, 0, -795, 0, 821, 0];
	trunk.Plane = 510; trunk->SetR(90);
	DrawMaterialQuad("Tunnel", 796, 591, 836, 591, 836, 602, 796, 602);
	
	return;
}

// The cave center has two lakes with seaweed.
private func InitCaveCenter()
{
	// Seaweed in the two lakes.
	Seaweed->Place(5, Rectangle(540, 480, 140, 100));
	Seaweed->Place(12, Rectangle(520, 330, 380, 100));
	
	// Some vegetation on the walls above first lake.
	var rank = CreateObject(Rank, 722, 437); 
	rank.MeshTransformation = [946, 0, 326, 0, 0, 1000, 0, 0, -326, 0, 946, 0];
	rank->SetR(200 + Random(30)); rank->DoCon(30); 
	rank = CreateObject(Rank, 648, 437); 
	rank.MeshTransformation = [956, 0, 292, 0, 0, 1000, 0, 0, -292, 0, 956, 0]; 	
	rank->SetR(165 + Random(30)); rank->DoCon(15); rank.Plane = 510;
	
	// Some vegetation between the two bodies of water.
	var ferns = Fern->Place(5 + Random(2), Rectangle(420, 370, 110, 90));
	for (var fern in ferns)
		fern->SetR(40 + Random(10));
	var mushrooms = Mushroom->Place(7 + Random(3), Rectangle(420, 370, 110, 90));
	for (var mushroom in mushrooms)
		mushroom->SetR(40 + Random(10));
		
	// Some vegetation on the walls above second lake.
	rank = CreateObject(Rank, 527, 290); 
	rank.MeshTransformation = [946, 0, 326, 0, 0, 1000, 0, 0, -326, 0, 946, 0];
	rank->SetR(165 + Random(30)); rank->DoCon(30); 
	rank = CreateObject(Rank, 579, 314); 
	rank.MeshTransformation = [956, 0, 292, 0, 0, 1000, 0, 0, -292, 0, 956, 0]; 	
	rank->SetR(165 + Random(30)); rank->DoCon(15); rank.Plane = 510;
	rank = CreateObject(Rank, 649, 305); 
	rank.MeshTransformation = [956, 0, 292, 0, 0, 1000, 0, 0, -292, 0, 956, 0]; 	
	rank->SetR(145 + Random(30)); rank->DoCon(15); 
	rank = CreateObject(Rank, 674, 310); 
	rank.MeshTransformation = [956, 0, 292, 0, 0, 1000, 0, 0, -292, 0, 956, 0]; 	
	rank->SetR(200 + Random(30)); rank->DoCon(30); rank.Plane = 510;
	
	
	return;
}

// Gold mine with dynamite and lorry.
private func InitGoldMine()
{
	// Foundry with a lorry and some metal.
	CreateObject(Foundry, 980, 486);
	CreateObject(Metal, 950, 474);
	
	// Pickaxe + ore near ore field.
	CreateObject(Lorry, 824, 460);
	CreateObject(Pickaxe, 808, 446)->SetR(RandomX(300, 330));
	CreateObject(Ore, 818, 460);
	CreateObject(Ore, 839, 460);
	
	// Shovel + coal chunks near coal field.
	CreateObject(Shovel, 966, 534)->SetR(RandomX(90, 135));
	CreateObject(Coal, 959, 532);
	CreateObject(Coal, 936, 532);
	
	// Dynamite box to blast through mine.
	var dyn1 = CreateObject(Dynamite, 959, 585, NO_OWNER); dyn1->SetR(-135);
	var dyn2 = CreateObject(Dynamite, 974, 597, NO_OWNER); dyn2->SetR(-105);
	var dyn3 = CreateObject(Dynamite, 970, 618, NO_OWNER); dyn3->SetR(-60);
	var dyn4 = CreateObject(Dynamite, 955, 633, NO_OWNER); dyn4->SetR(-30);
	var dyn5 = CreateObject(Dynamite, 941, 637, NO_OWNER); dyn5->SetR(-15);
	CreateObject(Fuse, 959, 585, NO_OWNER)->Connect(dyn1, dyn2);
	CreateObject(Fuse, 974, 597, NO_OWNER)->Connect(dyn2, dyn3);
	CreateObject(Fuse, 970, 618, NO_OWNER)->Connect(dyn3, dyn4);
	CreateObject(Fuse, 955, 633, NO_OWNER)->Connect(dyn4, dyn5);
	var igniter = CreateObject(Igniter, 909, 637, NO_OWNER);
	CreateObject(Fuse, 941, 637, NO_OWNER)->Connect(dyn5, igniter);
	igniter->SetGraphics("0", Fuse, 1, GFXOV_MODE_Picture);
				
	return;
}

// Cave exit: Elevator, cabin and flag.
private func InitCaveExit()
{
	// A small settlement.
	var elevator = CreateObject(Elevator, 872, 328);
	elevator->CreateShaft(248);
	CreateObject(WoodenCabin, 968, 328);
	
	// Some vegetation / rocks on top of the cave.
	SproutBerryBush->Place(1 + Random(2), Rectangle(420, 200, 160, 70));
	SproutBerryBush->Place(1, Rectangle(640, 80, 100, 70));	
	var bigrock = CreateObject(BigRock, 218, 395);
	bigrock.MeshTransformation = [-103, 561, -337, 0, 0, 730, 1216, 0, 555, 74, -44, 0];
	
	return;
}

// Vegetation throughout the scenario.
private func InitVegetation()
{
	PlaceGrass(85);
	PlaceObjects(Firestone, 25 + Random(5), "Earth");
	PlaceObjects(Loam, 15 + Random(5), "Earth");
	return;
}

private func InitAnimals()
{
	// The wipf as your friend, controlled by AI.
	CreateObject(Wipf, 76, 616);
	
	// Some butterflies as atmosphere.
	for (var i = 0; i < 25; i++)
		PlaceAnimal(Butterfly);
		
	return;
}	

/*-- Player Handling --*/

protected func InitializePlayer(int plr)
{
	var clonk = GetCrew(plr, 0);
	clonk->SetPosition(60, 606);
	var effect = AddEffect("ClonkRestore", clonk, 100, 10);
	effect.to_x = 60;
	effect.to_y = 606;
	
	// Add an effect to the clonk to track the goal.
	AddEffect("TrackGoal", clonk, 100, 2);

	// Standard player zoom for tutorials, player is not allowed to zoom in/out.
	SetPlayerViewLock(plr, true);
	//SetPlayerZoomByViewRange(plr, 400, nil, PLRZOOM_Direct | PLRZOOM_LimitMin | PLRZOOM_LimitMax);
	
	// Create tutorial guide, add messages, show first.
	guide = CreateObject(TutorialGuide, 0, 0 , plr);
	guide->AddGuideMessage("$MsgTutorialWelcome$");
	guide->AddGuideMessage("$MsgTutorialHUD$");
	guide->AddGuideMessage("$MsgTutorialFollowFriend$");
	guide->ShowGuideMessage(0);
	AddEffect("TutorialScale", nil, 100, 2);
	return;
}

/*-- Intro, Tutorial Goal & Outro --*/

private func OnFinishedTutorialIntro()
{
	// enable crew
	//target->SetCrewEnabled(true);
	//SetCursor(target->GetOwner(), target);
	
	// start the wipf
	FindObject(Find_ID(Wipf))->StartMoving();
}

global func FxTrackGoalTimer(object target, proplist effect, int time)
{
	if (Inside(target->GetX(), 980, 996) && Inside(target->GetY(), 314, 324))
	{
		if (FindObject(Find_ID(Wipf), Find_Distance(15, target->GetX(), target->GetY())))
		{
			AddEffect("GoalOutro", target, 100, 5);
			return -1;	
		}		
	}
	return 1;
}

global func FxGoalOutroStart(object target, proplist effect, int temp)
{
	if (temp)
		return;
		
	// Show guide message congratulating.
	guide->AddGuideMessage("$MsgTutorialCompleted$");
	guide->ShowGuideMessage(8);
		
	// Halt clonk and disable player controls.	
	target->SetCrewEnabled(false);
	target->SetComDir(COMD_Stop);
	
	// Find wipf and enter it into the clonk.	
	var wipf = FindObject(Find_ID(Wipf), Find_Distance(15, target->GetX(), target->GetY()));
	if (wipf)
		wipf->Enter(target);
	
	// Let the clonk pick up the wipf as an outro.
	effect.mesh = target->AttachMesh(Wipf, "pos_tool1", "Bone.006");
	effect.anim = target->PlayAnimation("CarryArmsPickup", 10, Anim_Linear(0,0,target->GetAnimationLength("CarryArmsPickup"), 60, ANIM_Remove), Anim_Const(1000));
		
	return 1;
}

global func FxGoalOutroTimer(object target, proplist effect, int time)
{
	if (time >= 60)
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
	
	target->DetachMesh(effect.mesh);
	target->StopAnimation(effect.anim);
}

/*-- Guide Messages --*/
// Finds when the Clonk has done 'X', and changes the message.

global func FxTutorialScaleTimer(object target, proplist effect, int timer)
{
	if (FindObject(Find_ID(Clonk), Find_InRect(264, 568, 80, 48)))
	{
		while (GetEffect("TutorialIntro*"))
			RemoveEffect("TutorialIntro*");
		guide->AddGuideMessage("$MsgTutorialScaleWall$");
		guide->ShowGuideMessage(3);
		AddEffect("TutorialHangle", nil, 100, 2);
		return -1;
	}
}

global func FxTutorialHangleTimer(object target, proplist effect, int timer)
{
	if (FindObject(Find_ID(Clonk), Find_InRect(480, 560, 80, 48)))
	{
		guide->AddGuideMessage("$MsgTutorialHangle$");
		guide->ShowGuideMessage(4);
		AddEffect("TutorialJump", nil, 100, 2);
		return -1;
	}
}

global func FxTutorialJumpTimer(object target, proplist effect, int timer)
{
	if (FindObject(Find_ID(Clonk), Find_InRect(744, 568, 56, 40)))
	{
		guide->AddGuideMessage("$MsgTutorialJump$");
		guide->ShowGuideMessage(5);
		AddEffect("TutorialSwimming", nil, 100, 2);
		return -1;
	}
}

global func FxTutorialSwimmingTimer(object target, proplist effect, int timer)
{
	if (FindObject(Find_ID(Clonk), Find_InRect(672, 440, 80, 72)))
	{
		guide->AddGuideMessage("$MsgTutorialSwimming$");
		guide->ShowGuideMessage(6);
		AddEffect("TutorialDiving", nil, 100, 2);
		return -1;
	}
}

global func FxTutorialDivingTimer(object target, proplist effect, int timer)
{
	if (FindObject(Find_ID(Clonk), Find_InRect(472, 280, 48, 64)))
	{
		guide->AddGuideMessage("$MsgTutorialDiving$");
		guide->ShowGuideMessage(7);
		return -1;
	}
}

protected func OnGuideMessageShown(int plr, int index)
{
	// Show the player his clonk and the guide.
	if (index == 0)
	{
		TutArrowShowTarget(GetCrew(GetPlayerByIndex()), 225, 24);
	}
	// Show the player HUD.
	if (index == 1)
	{
		TutArrowShowGUITarget(FindObject(Find_ID(GUI_CrewSelector)), 0); 	
		TutArrowShowGUITarget(FindObject(Find_ID(GUI_Goal)), 0); 	
	}
	// Show the clonks friend: the wipf.	
	if (index == 2)
	{
		OnFinishedTutorialIntro();
		TutArrowShowTarget(FindObject(Find_ID(Wipf))); 	
	}
	// Show wall to climb.
	if (index == 3)
		TutArrowShowPos(366, 570, 45);		
	// Show ceiling on which to hangle.
	if (index == 4)
		TutArrowShowPos(592, 564, 45);
	// Show wall to jump to.
	if (index == 5)
	{
		TutArrowShowPos(780, 560, 0);
		TutArrowShowPos(756, 544, 315);
	}
	// Show sea to swim in.
	if (index == 6)
		TutArrowShowPos(652, 478, 225);
	// Show breath indicator.
	if (index == 7)
		TutArrowShowGUITarget(FindObject(Find_ID(GUI_CrewSelector)), 0);
		
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
	if (FindObject(Find_OCF(OCF_CrewMember), Find_InRect(488, 280, 32, 48)))
	{
		effect.to_x = 500;
		effect.to_y = 318;             
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