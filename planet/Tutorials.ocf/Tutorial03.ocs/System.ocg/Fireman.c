// A fireman which puts out the fires around the village.

global func CreateFireman(int x, int y)
{
	var fireman = CreateObject(Clonk, x, y);
	fireman->SetSkin(2);
	fireman->SetDir(DIR_Left);
	fireman->SetColor(0xffaa55);
	fireman->SetName("Hugo");
	var barrel = fireman->CreateContents(Barrel);
	barrel->SetFilled("Water", 300);
	fireman->Message("<c ff0000>Fire!</c> Out of my way!");
	fireman->SetCommand("MoveTo", nil, 100, 374);
	

	AddEffect("IntFireman", fireman, 100, 5);
}

global func FxIntFiremanStart(object target, proplist effect)
{
	


	return FX_OK;
}

global func FxIntFiremanTimer(object target, proplist effect)
{
	if (!target->GetCommand())
		target->Contents(0)->ControlUse(target, -10, -10);


	return FX_OK;
}