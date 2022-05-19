#include "mario.h"
///////////////////////
//  Inital SpecialS  //
///////////////////////
/// SpecialS
/// 0x800E1450
///
void SpecialS(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;
	SpecialSFtCmd *script_flags = &fighter_data->ftcmd_var;
	SpecialSVar *state_var = &fighter_data->state_var;

	// stop y velocity
	fighter_data->phys.self_vel.Y = 0;

	// switch to special s state and update subaction
	ActionStateChange(0, 1, 0, gobj, STATE_SPECIALS, 0, 0);
	Fighter_AdvanceScript(gobj);

	// clear flags that are going to be used by this action
	script_flags->create_wind = 0;
	script_flags->enable_reflect = 0;
	script_flags->spawn_cape = 0;
	state_var->reflect_enabled = 0;

	// set the accessory callback for mario's cape
	// this function will spawn the cape item in mario's hand
	fighter_data->cb.Accessory4 = MarioCapeThink;
	return;
}
/// SpecialSAir
/// 0x800E14C8
///
void SpecialAirS(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;
	MarioAttr *mrAttr = fighter_data->special_attributes;
	SpecialSFtCmd *script_flags = &fighter_data->ftcmd_var;
	SpecialSVar *state_var = &fighter_data->state_var;

	// slow down x velocity by rate defined in special attributes
	fighter_data->phys.self_vel.X = fighter_data->phys.self_vel.X / mrAttr->specialS_horizontal_momentum;

	// change to aerial special s state and update subaction
	ActionStateChange(0, 1, 0, gobj, STATE_SPECIALSAIR, 0, 0);
	Fighter_AdvanceScript(gobj);

	// clear flags that are going to be used by this action
	script_flags->create_wind = 0;
	script_flags->enable_reflect = 0;
	script_flags->spawn_cape = 0;
	state_var->reflect_enabled = 0;

	// set the accessory callback for mario's cape
	// this function will spawn the cape item in mario's hand
	fighter_data->cb.Accessory4 = MarioCapeThink;
	return;
}
//////////////////////
//       Misc       //
//////////////////////
///
/// 0x802b2560
///
GOBJ *CreateCape(double facing_direction, GOBJ *fighter_gobj, Vec3 *position, int bone_index, int kind)
{
	// initialize spawn struct for cape
	SpawnItem spawnItem;
	spawnItem.parent_gobj = fighter_gobj;
	spawnItem.parent_gobj2 = fighter_gobj;
	spawnItem.it_kind = kind;
	spawnItem.pos.X = position->X;
	spawnItem.pos.Y = position->Y;
	spawnItem.pos.Z = position->Z;
	spawnItem.pos2.X = position->X;
	spawnItem.pos2.Y = position->Y;
	spawnItem.pos2.Z = position->Z;
	spawnItem.vel.X = 0;
	spawnItem.vel.Y = 0;
	spawnItem.vel.Z = 0;
	spawnItem.facing_direction = facing_direction;
	spawnItem.damage = 0;
	spawnItem.unk6 = 0;
	spawnItem.is_raycast_below = 1;

	// create cape item
	GOBJ *item = Item_CreateItem1(&spawnItem);

	// if item successfully spawned
	if (item != 0)
	{
		// have mario hold the item
		Item_Hold(item, fighter_gobj, bone_index);

		// copy develop mode stuff
		Item_CopyDevelopState(item, fighter_gobj);
	}
	return item;
}
///
///
///
void DestroyCape(GOBJ *gobj)
{
	Fighter_DestroyAndRemoveHeldFighterItem(gobj);
	return;
}
///
///
///
void UnknownCapeExitHitlag(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;

	// ft_var5 stores the cape item gobj
	if (fighter_data->fighter_var.ft_var5 != 0)
	{
		void (*UnknownItemFunction)(GOBJ * item_gobj) = (void *)0x8026b73c;

		UnknownItemFunction(fighter_data->fighter_var.ft_var5);
	}

	return;
}
///
///
///
void CapeEnableUnknownHitbox3Flag(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;

	// ft_var5 stores the cape item gobj
	if (fighter_data->fighter_var.ft_var5 != 0)
	{
		Item_EnableUnknownFlag(fighter_data->fighter_var.ft_var5);
	}
	return;
}
///
///
///
void MarioCapeThink(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;
	MarioAttr *mrAttr = fighter_data->special_attributes;
	SpecialSFtCmd *script_flags = &fighter_data->ftcmd_var;

	// this flag is used to track if the cape has been spawned already
	if (script_flags->spawn_cape == 0)
	{
		script_flags->spawn_cape = 1;

		// get the index of mario's right holding bone (RHaveN)
		int bone_index = Fighter_BoneLookup(fighter_data, RHaveN);

		// get the position of this bone
		Vec3 pos;
		JOBJ_GetWorldPosition(fighter_data->bones[bone_index].joint, 0, &pos);

		// create the cape item
		// cape kind was originally stored in mario's attributes
		int mex_cape_kind = MEX_GetFtItemID(gobj, MEX_ITEM_CAPE);
		GOBJ *cape = CreateCape(fighter_data->facing_direction, gobj, &pos, bone_index, mex_cape_kind); // mrAttr->cape_item_kind);

		// store the cape pointer to a ft_var5 and the special help item location
		fighter_data->fighter_var.ft_var5 = cape;
		fighter_data->item_held_spec = cape;

		// if the cape successully spawned, set the callbacks to remove it
		if (fighter_data->fighter_var.ft_var5 != 0)
		{
			fighter_data->cb.OnDeath_State = DestroyCape;
			fighter_data->cb.OnTakeDamage = DestroyCape;
		}

		//
		fighter_data->cb.EnterHitlag = CapeEnableUnknownHitbox3Flag;
		fighter_data->cb.ExitHitlag = UnknownCapeExitHitlag;

		// clear the accessory callback so this function will no longer be called
		fighter_data->cb.Accessory4 = (void *)0x0;
	}
	return;
}
///////////////////////
// Grounded SpecialS //
///////////////////////
///
/// 0x800E1550
///
void SpecialS_AnimationCallback(GOBJ *gobj)
{
	// return to wait animation after animation ends
	if (FrameTimerCheck(gobj) == 0)
	{
		Fighter_EnterWait(gobj);
	}

	return;
}
///
/// 0x800E15C8
///
void SpecialS_IASACallback(GOBJ *gobj)
{
	return;
}
///
/// 0x800E15D0
///
void SpecialS_PhysicCallback(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;
	MarioAttr *mrAttr = fighter_data->special_attributes;
	SpecialSVar *state_var = &fighter_data->state_var;
	SpecialSFtCmd *script_flags = &fighter_data->ftcmd_var;

	// ftCmd sets this subaction flag
	// creates wind when it is set
	if (script_flags->create_wind == 1)
	{
		script_flags->create_wind = 2;

		// i'm not sure why the Hip bone, but that's what it uses
		int bone_index = Fighter_BoneLookup(fighter_data, HipN);

		// get this bones position
		Vec3 bone_position;
		JOBJ_GetWorldPosition(fighter_data->bones[bone_index].joint, 0, &bone_position);

		// move the position forward a little
		bone_position.X += 3 * fighter_data->facing_direction;

		// create wind at this position
		Wind_FighterCreate(&bone_position, 0.9f, 0.02f, 1.0471976f, 0x78);
	}

	// apply friction
	Fighter_PhysGround_ApplyFriction(gobj);

	// create the reflect bubble when flags are set
	if ((script_flags->enable_reflect == 1) && (state_var->reflect_enabled == 0))
	{
		state_var->reflect_enabled = 1;
		Fighter_CreateReflect(gobj, &mrAttr->reflect_data, 0);
	}
	else if ((script_flags->enable_reflect == 0) && (state_var->reflect_enabled == 1))
	{
		state_var->reflect_enabled = 0;
		fighter_data->flags.reflect_enable = 0;
	}

	// update the reflect so that it follows fighter
	Fighter_EnableReflectUpdate(gobj);
	return;
}
///
/// 0x800e18b8
///
void SpecialS_EnterAir(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;
	SpecialSVar *state_var = &fighter_data->state_var;
	SpecialSFtCmd *script_flags = &fighter_data->ftcmd_var;

	// set fighter to air state
	Fighter_SetAirborne(fighter_data);

	// enter air state
	ActionStateChange(fighter_data->state.frame, 1, 0, gobj, STATE_SPECIALSAIR, 0xc4c508c, 0);

	//
	if (script_flags->create_wind == 1)
	{
		script_flags->create_wind = 2;
	}

	//
	if (state_var->reflect_enabled != 0)
	{
		fighter_data->flags.reflect_enable = 1;
	}

	// if cape is set then make sure the damage callbacks are set
	// these callbacks will remove the cape when mario is hit or dies
	// ft_var5 stores the cape item gobj
	if (fighter_data->fighter_var.ft_var5 != 0)
	{
		fighter_data->cb.OnDeath_State = DestroyCape;
		fighter_data->cb.OnTakeDamage = DestroyCape;
	}

	//
	fighter_data->cb.EnterHitlag = CapeEnableUnknownHitbox3Flag;
	fighter_data->cb.ExitHitlag = UnknownCapeExitHitlag;

	// cape is created in this function
	fighter_data->cb.Accessory4 = MarioCapeThink;
	return;
}
///
/// 0x800E1840
///
void SpecialS_CollisionCallback(GOBJ *gobj)
{
	// enter aerial state when falling off ledge
	if (Fighter_CollGround_StopLedge(gobj) == 0)
	{
		SpecialS_EnterAir(gobj);
	}

	return;
}
///////////////////////
//  Aerial SpecialS  //
///////////////////////
///
/// 0x800E158C
///
void SpecialAirS_AnimationCallback(GOBJ *gobj)
{
	// enter fall state when animation ends
	if (FrameTimerCheck(gobj) == 0)
	{
		Fighter_EnterFall(gobj);
	}

	return;
}
///
/// 0x800E15CC
///
void SpecialAirS_IASACallback(GOBJ *gobj)
{
	return;
}
///
/// 0x800E16E0
///
void SpecialAirS_PhysicCallback(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;
	MarioAttr *mrAttr = fighter_data->special_attributes;
	SpecialSVar *state_var = &fighter_data->state_var;
	SpecialSFtCmd *script_flags = &fighter_data->ftcmd_var;

	if (script_flags->create_wind == 0)
	{
		Fighter_Phys_ApplyVerticalAirFriction(fighter_data);
	}
	else
	{
		if (script_flags->create_wind == 1)
		{
			script_flags->create_wind = 2;
			if (fighter_data->fighter_var.ft_var4 == 0)
			{
				fighter_data->fighter_var.ft_var4 = 1;
				fighter_data->phys.self_vel.Y = mrAttr->specialS_vertical_momentum;
			}
			else
			{
				fighter_data->phys.self_vel.Y = 0;
			}
			int bone_index = Fighter_BoneLookup(fighter_data, 4);

			Vec3 pos;
			JOBJ_GetWorldPosition(fighter_data->bones[bone_index].joint, 0, &pos);

			pos.X += 3 * fighter_data->facing_direction;

			Wind_FighterCreate(&pos, 0.9f, 0.1f, 1.0471976f, 0x78);
		}

		Fighter_PhysAir_ApplyGravity(fighter_data, mrAttr->specialS_gravity, mrAttr->specialS_gravity_limit);
	}

	Fighter_PhysAir_DecayXVelocity(fighter_data, mrAttr->specialS_horizontal_velocity);

	if ((script_flags->enable_reflect == 1) && (state_var->reflect_enabled == 0))
	{
		state_var->reflect_enabled = 1;
		Fighter_CreateReflect(gobj, &mrAttr->reflect_data, 0);
	}
	else if ((script_flags->enable_reflect == 0) && (state_var->reflect_enabled == 1))
	{
		state_var->reflect_enabled = 0;
		fighter_data->flags.reflect_enable = 0;
	}

	Fighter_EnableReflectUpdate(gobj);
	return;
}
///
/// 0x800e198c
///
void SpecialAirS_CollisionCallback_StateChange(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;
	SpecialSVar *state_var = &fighter_data->state_var;

	fighter_data->fighter_var.ft_var4 = 0;

	// ground fighter
	Fighter_SetGrounded2(fighter_data);

	// change into grounded special state and preserve frame
	ActionStateChange(fighter_data->state.frame, 1, 0, gobj, STATE_SPECIALS, 0xc4c508c, 0);

	// enable reflect flag if stateVar has been set
	if (state_var->reflect_enabled != 0)
	{
		fighter_data->flags.reflect_enable = 1;
	}

	// ft_var5 stores the cape item gobj
	if (fighter_data->fighter_var.ft_var5 != 0)
	{
		fighter_data->cb.OnDeath_State = DestroyCape;
		fighter_data->cb.OnTakeDamage = DestroyCape;
	}

	// re-set enter and exit hitlag events
	fighter_data->cb.EnterHitlag = CapeEnableUnknownHitbox3Flag;
	fighter_data->cb.ExitHitlag = UnknownCapeExitHitlag;

	// re-set cape accessory callback
	fighter_data->cb.Accessory4 = MarioCapeThink;
	return;
}
///
/// 0x800E187C
///
void SpecialAirS_CollisionCallback(GOBJ *gobj)
{
	// if fighter collides with ground switch to grounded state
	if (Fighter_CollAir_IgnoreLedge_NoCB(gobj) != 0)
	{
		SpecialAirS_CollisionCallback_StateChange(gobj);
	}
	return;
}
