/** Tutorial Guide
* The tutorial guide can be clicked by the player, it supplies the player with information and hints.
* The following callbacks are made to the scenario script:
* - \c OnGuideMessageShown(int plr, int index) when a message is shown
* - \c OnGuideMessageRemoved(int plr, int index) when a message is removed, all events
* @author Maikel
*/


local messages; // A container to hold all messages.
local message_index; // Progress in reading messages.
local message_open; // current open message.

protected func Initialize()
{
	// Parallaxity
	this["Parallaxity"] = [0, 0];
	// Visibility
	this["Visibility"] = VIS_Owner;
	messages = [];
	message_index = nil;
	message_open = nil;
	
	// Initialize menu properties.
	InitializeMenu();
	
	// create the main menu
	UpdateGuideMenu("");	
	return;
}

/* Adds a message to the guide. The internal index is set to this message meaning that this message will
* be shown if the player clicks the guide.
* @param msg Message that should be added to the message stack.
*/
public func AddGuideMessage(string msg)
{
	// Automatically set index to current.
	message_index = GetLength(messages);
	// Add message to list.
	messages[message_index] = msg;
	// Update the menu, because of the next button.
	if (message_open != nil)
		ShowGuideMenu(message_open);
	// Make visible that there is a new message.
	AddEffect("NotifyPlayer", this, 100, 8, this);
	return;
}

/* Shows a guide message to the player, also resets the internal index to that point.
*	@param show_index The message corresponding to this index will be shown.
*/
public func ShowGuideMessage(int show_index)
{
	message_index = Max(0, show_index);
	ShowGuideMenu(message_index);
	if (GetEffect("NotifyPlayer", this))
		RemoveEffect("NotifyPlayer", this);
	// Increase index if possible.	
	if (GetLength(messages) > message_index + 1)
		message_index++;
	return;
}

/* Menu implementation */

// Menu IDs.
local id_menu;
local id_guide;
local id_text;
local id_next;
local id_prev;

// Menu proplists.
local prop_menu;
local prop_guide;
local prop_text;
local prop_next;
local prop_prev;

private func InitializeMenu()
{
	var menu_width = 25; // in percentage of whole screen
	var menu_height = 10; // in em, should correspond to five lines of text
	var text_margin = 2; // margins in 1/10 em
	
	// Menu IDs.
	id_guide = 1;
	id_text = 2;
	id_next = 3;
	id_prev = 4;

	// Submenu proplists.
	prop_guide = 
	{
		Target = this,
		ID = id_guide,
		Left = "0%",
		Right = Format("0%%+%dem", menu_height),
		Top = "0%",
		Bottom = Format("0%%+%dem", menu_height),
		Symbol = GetID(),
		BackgroundColor = {Std = 0, Hover = 0x50ffffff},
		OnMouseIn = GuiAction_SetTag("Hover"),
		OnMouseOut = GuiAction_SetTag("Std"),
		OnClick = GuiAction_Call(this, "ShowCurrentMessage"),
	};
	prop_text = 
	{
		Target = this,
		ID = id_text,
		Left = Format("0%%%s", ToEmString(10 * menu_height + text_margin)),
		Right = Format("100%%%s", ToEmString(- 5 * menu_height - text_margin)),
		Top = "0%",
		Bottom = "100%",
		Text = "",
		BackgroundColor = {Std = 0},	
	};	
	prop_next =
	{
		Target = this,
		ID = id_next,
		Left = Format("100%%-%dem", menu_height / 2),
		Right = "100%",
		Top = Format("100%%-%dem", menu_height / 2),
		Bottom = "100%",
		Symbol = Icon_Exit,
		//Text = "$MsgNext$",
		BackgroundColor = {Std = 0, Hover = 0x50ffff00},
		OnMouseIn = GuiAction_SetTag("Hover"),
		OnMouseOut = GuiAction_SetTag("Std"),
		OnClick = GuiAction_Call(this, "ShowNextMessage"),
	};
	prop_prev =
	{
		Target = this,
		ID = id_prev,
		Left = Format("100%%-%dem", menu_height / 2),
		Right = "100%",
		Top = "0%", 
		Bottom = Format("0%%+%dem", menu_height / 2),
		Symbol = Icon_Enter,
		//Text = "$MsgPrevious$",
		BackgroundColor = {Std = 0, Hover = 0x50ffff00},
		OnMouseIn = GuiAction_SetTag("Hover"),
		OnMouseOut = GuiAction_SetTag("Std"),
		OnClick = GuiAction_Call(this, "ShowPreviousMessage"),
	};
	
	// Menu proplist.
	prop_menu =
	{
		Target = this,
		Style = GUI_Multiple,
		Decoration = GUI_MenuDeco,
		Left = Format("%d%%", 50 - menu_width),
		Right = Format("%d%%", 50 + menu_width),
		Top = "0%+2em",
		Bottom = Format("0%%+%dem", menu_height + 2),
		BackgroundColor = {Std = 0},
	};
	prop_menu.guide = prop_guide;
	prop_menu.text = prop_text;
	prop_menu.next = prop_next;
	prop_menu.prev = prop_prev;
	
	// Menu ID.
	id_menu = CustomGuiOpen(prop_menu);
}

private func ShowGuideMenu(int index)
{
	// There will always be the removal of the previous message.
	if (message_open != nil)
		GameCall("OnGuideMessageRemoved", GetOwner(), message_open);

	// Show the new message.
	var message = messages[index];
	if (!message)
		return;
	var has_next = index < GetLength(messages) - 1;
	var has_prev = index > 0;			
	UpdateGuideMenu(message, has_next, has_prev);
	message_open = index;

	// Notify the scenario script.
	if (message_open != nil)	
		GameCall("OnGuideMessageShown", GetOwner(), message_open);
}

private func UpdateGuideMenu(string guide_message, bool has_next, bool has_prev)
{
	CustomGuiClose(id_menu, 0, this);

	prop_text.Text = guide_message;
	prop_menu.text = prop_text;
	if (has_next)
		prop_menu.next = prop_next;
	else
		prop_menu.next = nil;
	if (has_prev)
		prop_menu.prev = prop_prev;
	else
		prop_menu.prev = nil;	
		
	id_menu = CustomGuiOpen(prop_menu);	
	
	/*// Update the guide's message.
	prop_text.Text = guide_message;
	CustomGuiUpdate(prop_text, id_menu, id_text, this);
	
	// Update next and previous buttons.
	if (has_next)
	{
		CustomGuiUpdate(prop_next, id_menu, id_next, this);
	}
	else
	{	
		CustomGuiClose(id_menu, id_next, this);
	}
	
	if (has_prev)
	{
		CustomGuiUpdate({prop_new = prop_prev}, id_menu, id_prev, this);
	}
	else
	{	
		CustomGuiClose(id_menu, id_prev, this);
	}*/
}

// Menu callback: the player has clicked on the guide.
private func ShowCurrentMessage()
{
	if (GetEffect("NotifyPlayer", this))
		RemoveEffect("NotifyPlayer", this);
	// Show guide message if there is a new one.
	ShowGuideMenu(message_index);
	// Increase index if possible.
	if (GetLength(messages) > message_index + 1)
		message_index++;
	return;
}

// Menu callback: the player has clicked on next message.
public func ShowNextMessage()
{
	if (message_open >= GetLength(messages) - 1)
		return;
	
	ShowGuideMenu(message_open + 1);
}

// Menu callback: the player has clicked on previos message.
public func ShowPreviousMessage()
{
	if (message_open == 0)
		return;
		
	ShowGuideMenu(message_open - 1);
}

// Effect to display the player notification for some time.
protected func FxNotifyPlayerStart(object target, effect, int temporary)
{
	effect.On = true;
	return 1;
}

protected func FxNotifyPlayerTimer(object target, effect, int time)
{
	if (effect.On)
	{
		effect.On = false;
		prop_guide.BackgroundColor = {Std = 0x50ffffff, Hover = 0x50ffffff};
	}
	else
	{
		effect.On = true;
		prop_guide.BackgroundColor = {Std = 0, Hover = 0x50ffffff};
	}
	//Log("notify: %v", effect.On);
	CustomGuiUpdate(prop_guide, id_menu, id_guide, this);

	// Delete effect if time has passed.
	if (time > 2 * 36)
		return -1;
	return 1;
}

protected func FxNotifyPlayerStop(object target, effect, int reason, bool temporary)
{
	prop_guide.BackgroundColor = {Std = 0, Hover = 0x50ffffff};
	CustomGuiUpdate(prop_guide, id_menu, id_guide, this);
	return 1;
}

protected func FxNotifyPlayerEffect(string new_name)
{
	if (new_name == "NotifyPlayer") 
		return -1;
}

local Name = "$Name$";

