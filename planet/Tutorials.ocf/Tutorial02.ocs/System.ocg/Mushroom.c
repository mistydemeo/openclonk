// Mushrooms are loose and don't need to be picked.

#appendto Mushroom

protected func Initialize()
{	
	this.Collectible = 1;
	return _inherited(...);
}