#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include <stage.h>
#include <profile.h>
#include "boss.h"

//Yeah... I know... This is a lazy copy of the sidestepper boss... But it works! -LiQ | 02/06/21 11:13am


const char* MiniSidestepperArcNameList[] =
{

	"test_lift",
	NULL
};

class daMiniSidestepper_c : public dEn_c {
public:

	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	m3d::mdl_c bodyModel;
	nw4r::g3d::ResFile resFile;
	m3d::anmChr_c chrAnimation;

	u64 eventFlag;
	u32 delay;

	u32 effect;
	u8 type;

	S16Vec nullRot;
	Vec efScale;


	dAc_Py_c* target;

	float BaseLine;
	int randomnum;
	int isBuried; //0 when not, 1 when yes
	int startposx;
	bool left;
	bool right;
	bool fastwalkafterhit;
	bool everysecondtry;
	float rndmnum;
	int plusorminus; // 0-Plus, 1-Minus
	int plusorminusrock; // 0-Plus, 1-Minus
	int rndmactor;
	int buryprojectiletimer;
	bool everysecondtry2;
	int walkwaitwalk; //for wait 
	bool left2;
	bool right2;
	int timerock;
	int rndmtimerock;
	bool morelives;
	float point0;
	float point1;
	float point2;
	int whichpoint;
	
	int timer;

	int lives;


	Vec possand;
	Vec BackUpEffect;
	Vec posbarrel;
	Vec posenemy;
	Vec posrock;
	Vec barreleffect;
	Vec enemyeffect;


	static dActor_c* build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);

	void updateModelMatrices();
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther);

	bool collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat5_Mario(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat8_FencePunch(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat11_PipeCannon(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther);
	bool collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther);

	USING_STATES(daMiniSidestepper_c);
	DECLARE_STATE(Wait);
	DECLARE_STATE(Walk);
	DECLARE_STATE(Bury);
	DECLARE_STATE(MoveSand);
	DECLARE_STATE(BackUp);
	DECLARE_STATE(Outro);
};

CREATE_STATE(daMiniSidestepper_c, Wait);
CREATE_STATE(daMiniSidestepper_c, Walk);
CREATE_STATE(daMiniSidestepper_c, Bury);
CREATE_STATE(daMiniSidestepper_c, MoveSand);
CREATE_STATE(daMiniSidestepper_c, BackUp);
CREATE_STATE(daMiniSidestepper_c, Outro);

void daMiniSidestepper_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->chrAnimation.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->chrAnimation, unk2);
	this->chrAnimation.setUpdateRate(rate);
}


// Extra collision conditions:

void daMiniSidestepper_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther)
{
	this->_vf220(apOther->owner);
}

bool daMiniSidestepper_c::collisionCat3_StarPower(ActivePhysics* apThis, ActivePhysics* apOther) {
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat5_Mario(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player collides using butt slide
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCatD_Drill(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player drills with propeller
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCat8_FencePunch(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player punches fence behind
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCat7_GroundPound(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player ground pounds
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCat7_GroundPoundYoshi(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player ground pounds with Yoshi
	playerCollision(apThis, apOther);
	return true;
}
bool daMiniSidestepper_c::collisionCatA_PenguinMario(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player slides with penguin suit
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat11_PipeCannon(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player shoots from pipe cannon
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat9_RollingObject(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When player throws object at sprite

	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat1_Fireball_E_Explosion(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When collides with fireball
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When collides with ice ball
	return true;
}
bool daMiniSidestepper_c::collisionCat13_Hammer(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When collides with hammer projectile
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}
bool daMiniSidestepper_c::collisionCat14_YoshiFire(ActivePhysics* apThis, ActivePhysics* apOther) {
	// When collides with Yoshi fire spit
	PlaySound(this, SE_EMY_DOWN);

	doStateChange(&StateID_Outro);

	return true;
}


dActor_c* daMiniSidestepper_c::build() {
	void* buffer = AllocFromGameHeap1(sizeof(daMiniSidestepper_c));
	return new(buffer) daMiniSidestepper_c;
}

const SpriteData MiniSidestepperSpriteData =
{ ProfileId::MiniSidestepper, 0, 0 , 0 , 0, 0x100, 0x100, 0, 0, 0, 0, 2 };

Profile MiniSidestepperProfile(&daMiniSidestepper_c::build, SpriteId::MiniSidestepper, MiniSidestepperSpriteData, ProfileId::MiniSidestepper, ProfileId::MiniSidestepper, "MiniSidestepper", MiniSidestepperArcNameList);


int daMiniSidestepper_c::onCreate() {
	//Variables
	this->nullRot = (S16Vec){ 0, 0, 0 };
	this->efScale = (Vec){ 1.0f, 0.5f, 1.0f };

	target = GetSpecificPlayerActor(0); //target is mario


	this->randomnum = 0;
	this->isBuried = 0; //0 when not, 1 when yes
	this->fastwalkafterhit = false;
	this->everysecondtry = false;
	this->everysecondtry2 = true;



	// Do the following once when object is spawned:

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	// Model settings. First string is arc name, second string is brres name inside arc, third string is mdl0 name inside brres.
	this->resFile.data = getResource("test_lift", "g3d/test_lift.brres");

	nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("test_lift");
	bodyModel.setup(mdl, &this->allocator, 0x224, 1, 0);
	SetupTextures_MapObj(&bodyModel, 0);

	nw4r::g3d::ResAnmChr anmChr = resFile.GetResAnmChr("wait");
	this->chrAnimation.setup(mdl, anmChr, &this->allocator, 0);


	//
	/*

	mdl = this->resFile.GetResMdl("bubble_fog_CS");
	fogModel.setup(mdl, &allocator, 0x124, 1, 0);

	nw4r::g3d::ResAnmTexSrt anmRes = this->resFile.GetResAnmTexSrt("wait_proj");
	this->fogSRT.setup(mdl, anmRes, &this->allocator, 0, 1);
	this->fogSRT.bindEntry(&this->fogModel, anmRes, 0, 0);
	this->fogModel.bindAnim(&this->fogSRT, 0.0);

	*/
	//



	allocator.unlink();

	// Physics settings. Use hitbox debug mode to test hitbox, [Press "-" 16 times on star coin menu].
	ActivePhysics::Info Physics;
	Physics.xDistToCenter = 0.0;
	Physics.yDistToCenter = 2.5;
	Physics.xDistToEdge = 10.0;
	Physics.yDistToEdge = 12.0;
	Physics.category1 = 0x3;
	Physics.category2 = 0x0;
	Physics.bitfield1 = 0x4F;
	Physics.bitfield2 = 0xFFFFFFFF;
	Physics.unkShort1C = 0;
	Physics.callback = &dEn_c::collisionCallback;

	this->aPhysics.initWithStruct(this, &Physics);
	this->aPhysics.addToList();

	// Size settings

	this->scale.x = 0.15;
	this->scale.y = 0.15;
	this->scale.z = 0.15;


	this->pos.z = -1000.0;

	this->startposx = pos.x;
	this->BaseLine = pos.y;

	this->point0 = this->startposx;
	this->point1 = this->startposx + 48.0;
	this->point2 = this->startposx - 48.0;

	doStateChange(&StateID_Wait);

	this->onExecute();
	return true;
}


int daMiniSidestepper_c::onDelete() {
	// Do the following when deleted:

	return true;
}

int daMiniSidestepper_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daMiniSidestepper_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daMiniSidestepper_c::onExecute() {
	// Do the following every frame:
	acState.execute();
	updateModelMatrices();
	bodyModel._vf1C();

	if (this->chrAnimation.isAnimationDone()) {
		this->chrAnimation.setCurrentFrame(0.0);
	}


	return true;
}



/*//////*/
/*States*/
/*//////*/



void daMiniSidestepper_c::beginState_Wait()
{
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
}

void daMiniSidestepper_c::executeState_Wait()
{
	this->timer += 1;

	if (timer > 50)
	{
		if (this->isBuried == 1)
		{
			this->isBuried = 0;
			
			everysecondtry2 = true;
			doStateChange(&StateID_MoveSand);
			

		}
		else
			doStateChange(&StateID_Walk);
	}
}

void daMiniSidestepper_c::endState_Wait()
{ }

void daMiniSidestepper_c::beginState_Walk()
{
	this->timer = 0;
	this->randomnum += 1;

	if (target->pos.x > pos.x)
	{
		bindAnimChr_and_setUpdateRate("walk_r", 1, 0.0, 1.0);
		this->right = true;
		this->left = false;
	}
	else if (target->pos.x < pos.x)
	{
		bindAnimChr_and_setUpdateRate("walk_l", 1, 0.0, 1.0);
		this->left = true;
		this->right = false;
	}
}

void daMiniSidestepper_c::executeState_Walk()
{
	this->timer += 1;


	if (this->left == true)
		pos.x -= 0.4;
	else if (this->right == true)
		pos.x += 0.4;
	if (this->randomnum == 3)
	{
		doStateChange(&StateID_Bury);
		this->randomnum = 0;
	}


	if (this->timer > 50)
		doStateChange(&StateID_Wait);
}

void daMiniSidestepper_c::endState_Walk()
{

}


void daMiniSidestepper_c::beginState_Bury()
{
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);

	this->timer = 0;

	PlaySound(this, 0x221);

	SpawnEffect("Wm_mr_sanddive_out", 0, &this->pos, &nullRot, &efScale);
	SpawnEffect("Wm_mr_sanddive_smk", 0, &this->pos, &nullRot, &efScale);
}

void daMiniSidestepper_c::executeState_Bury()
{
	if (this->timer < 60) {
		this->pos.y -= 2.0;  // Height is 54 pixels, move down
	}

	if (this->timer > 90) {
		this->isBuried = 1;
		doStateChange(&StateID_Wait);
	}

	this->timer += 1;
}

void daMiniSidestepper_c::endState_Bury()
{ }


void daMiniSidestepper_c::beginState_MoveSand()
{


	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	this->timer = 0;

}

void daMiniSidestepper_c::executeState_MoveSand()
{
	this->timer += 1;

	if (this->timer == 60)
	{

		this->possand = (Vec){ pos.x, this->BaseLine, 0 };

		SpawnEffect("Wm_mr_sanddive_out", 0, &this->possand, &nullRot, &efScale);
		SpawnEffect("Wm_mr_sanddive_smk", 0, &this->possand, &nullRot, &efScale);
	}

	if (this->timer == 120)
	{
		whichpoint = GenerateRandomNumber(3);
		switch (whichpoint)
		{
		case 0:
			pos.x = point0;
			break;
		case 1:
			pos.x = point1;
			break;
		case 2:
			pos.x = point2;
			break;
		}

		doStateChange(&StateID_BackUp);
	}
}

void daMiniSidestepper_c::endState_MoveSand()
{

}


void daMiniSidestepper_c::beginState_BackUp()
{
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);
	this->timer = 0;

	BackUpEffect = (Vec){ pos.x, this->BaseLine, 0 };


	SpawnEffect("Wm_mr_sanddive_out", 0, &this->BackUpEffect, &nullRot, &efScale);
	SpawnEffect("Wm_mr_sanddive_smk", 0, &this->BackUpEffect, &nullRot, &efScale);
}

void daMiniSidestepper_c::executeState_BackUp()
{
	if (this->timer < 60) {
		this->pos.y += 2.0;  // Height is 54 pixels, move down
	}

	if (this->timer > 90) {

		doStateChange(&StateID_Walk);
	}

	this->timer += 1;
}

void daMiniSidestepper_c::endState_BackUp()
{ }


void daMiniSidestepper_c::beginState_Outro()
{
	this->timer = 0;
	bindAnimChr_and_setUpdateRate("wait", 1, 0.0, 1.0);

	pos.z = 5000.0;
}

void daMiniSidestepper_c::executeState_Outro()
{
	if (timer == 300)
	{
		this->Delete(1);
	}
	
	this->rot.x += 0x500;
	this->rot.y += 0x500;
	this->rot.z += 0x500;
	pos.y -= 1.5;
	
	this->timer += 1;
}

void daMiniSidestepper_c::endState_Outro() { }
