// Adjusts the construction site of the wipf statue to facilite dropping gold bars.
// The gold bar effectively replaces the metal in the structure.

#appendto ConstructionSite

public func RejectCollect(id def, object obj)
{
	if (definition != WipfStatue || (def != GoldBar && def != Chunk_Metal))
		return _inherited(def, obj);

	return ContentsCount(GoldBar) + ContentsCount(Metal) >= 1;
}


private func GetMissingComponents()
{
	if (definition != WipfStatue)
		return _inherited();
		
	var missing_material = [{id = Chunk_Wood, count = 2}, {id = Chunk_Metal, count = 1}];
		
	full_material = true;

	for (var i = 0; i < 2; i++)
	{
		var mat = missing_material[i].id;
		var cnt = missing_material[i].count - ContentsCount(mat);
		// Substract gold bar count from metal amount as well.
		if (missing_material[i].id == Metal)
			cnt -= ContentsCount(GoldBar);
		missing_material[i].count = cnt;
		if (cnt > 0)
			full_material = false;
	}

	return missing_material;
}

private func StartConstructing()
{
	if(!definition)
		return;
	if(!full_material)
		return;
	
	// find all objects on the bottom of the area that are not stuck
	var wdt = GetObjWidth();
	var hgt = GetObjHeight();
	var lying_around = FindObjects(Find_Or(Find_Category(C4D_Vehicle), Find_Category(C4D_Object), Find_Category(C4D_Living)),Find_InRect(-wdt/2 - 2, -hgt, wdt + 2, hgt + 12), Find_OCF(OCF_InFree));
	
	// create the construction
	var site;
	if(!(site = CreateConstruction(definition, 0, 0, GetOwner(), 1, 1, 1)))
	{
		// spit out error message. This could happen if the landscape changed in the meantime
		// a little hack: the message would immediately vanish because this object is deleted. So, instead display the
		// message on one of the contents.
		if(Contents(0))
			CustomMessage("$TxtNoConstructionHere$", Contents(0), GetOwner(), nil,nil, RGB(255,0,0));
		Interact(nil, 1);
		return;
	}
	
	if(direction)
		site->SetDir(direction);
	// Inform about sticky building
	if (stick_to)
		site->CombineWith(stick_to);
	
	// Autoconstruct 2.0!
	Schedule(site, "DoCon(2)",1,50);
	Schedule(this,"RemoveObject()",1);
	
	if (definition == WipfStatue)
	{
		if (ContentsCount(GoldBar) >= 1)
			site->SetClrModulation(RGB(255, 255, 255));		
		if (ContentsCount(Chunk_Metal) >= 1)
			site->SetClrModulation(RGB(30, 50, 255));
	}
	
	// clean up stuck objects
	for(var o in lying_around)
	{
		var x, y;
		var dif = 0;
		
		x = o->GetX();
		y = o->GetY();
		
		// move living creatures upwards till they stand on top.
		if(o->GetOCF() & OCF_Alive)
		{
			while(o->GetContact(-1, CNAT_Bottom))
			{
				// only up to 20 pixel
				if(dif > 20)
				{
					o->SetPosition(x,y);
					break;
				}
				
				dif++;
				o->SetPosition(x, y-dif);
			}
		}
		else {
			while(o->Stuck())
			{
				// only up to 20 pixel
				if(dif > 20)
				{
					o->SetPosition(x,y);
					break;
				}
				
				dif++;
				o->SetPosition(x, y-dif);
			}
		}
	}
}
