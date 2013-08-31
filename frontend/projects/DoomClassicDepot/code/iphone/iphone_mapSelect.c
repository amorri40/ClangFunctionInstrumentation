/*
 Copyright (C) 2009-2011 id Software LLC, a ZeniMax Media company.
 Copyright (C) 2009 Id Software, Inc.
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 
 */


#include "../doomiphone.h"

typedef struct {
	int		dataset;
	int		episode;
	int		map;
	const char *name;
} mapData_t;

mapData_t mapData[] = {
{ 0, 1, 1,    "E1M1: Hangar" },
{ 0, 1, 2,    "E1M2: Nuclear Plant" },
{ 0, 1, 3,    "E1M3: Toxin Refinery" },
{ 0, 1, 4,    "E1M4: Command Control" },
{ 0, 1, 5,    "E1M5: Phobos Lab" },
{ 0, 1, 6,    "E1M6: Central Processing" },
{ 0, 1, 7,    "E1M7: Computer Station" },
{ 0, 1, 8,    "E1M8: Phobos Anomaly" },
{ 0, 1, 9,    "E1M9: Military Base" },

{ 0, 2, 1,    "E2M1: Deimos Anomaly" },
{ 0, 2, 2,    "E2M2: Containment Area" },
{ 0, 2, 3,    "E2M3: Refinery" },
{ 0, 2, 4,    "E2M4: Deimos Lab" },
{ 0, 2, 5,    "E2M5: Command Center" },
{ 0, 2, 6,    "E2M6: Halls of the Damned" },
{ 0, 2, 7,    "E2M7: Spawning Vats" },
{ 0, 2, 8,    "E2M8: Tower of Babel" },
{ 0, 2, 9,    "E2M9: Fortress of Mystery" },

{ 0, 3, 1,    "E3M1: Hell Keep" },
{ 0, 3, 2,    "E3M2: Slough of Despair" },
{ 0, 3, 3,    "E3M3: Pandemonium" },
{ 0, 3, 4,    "E3M4: House of Pain" },
{ 0, 3, 5,    "E3M5: Unholy Cathedral" },
{ 0, 3, 6,    "E3M6: Mt. Erebus" },
{ 0, 3, 7,    "E3M7: Limbo" },
{ 0, 3, 8,    "E3M8: Dis" },
{ 0, 3, 9,    "E3M9: Warrens" },

{ 0, 4, 1,    "E4M1: Hell Beneath" },
{ 0, 4, 2,    "E4M2: Perfect Hatred" },
{ 0, 4, 3,    "E4M3: Sever The Wicked" },
{ 0, 4, 4,    "E4M4: Unruly Evil" },
{ 0, 4, 5,    "E4M5: They Will Repent" },
{ 0, 4, 6,    "E4M6: Against Thee Wickedly" },
{ 0, 4, 7,    "E4M7: And Hell Followed" },
{ 0, 4, 8,    "E4M8: Unto The Cruel" },
{ 0, 4, 9,    "E4M9: Fear" },

#if 0

{ 0, 0, 0,       "level 1: entryway" },
{ 0, 0, 0,       "level 2: underhalls" },
{ 0, 0, 0,       "level 3: the gantlet" },
{ 0, 0, 0,       "level 4: the focus" },
{ 0, 0, 0,       "level 5: the waste tunnels" },
{ 0, 0, 0,       "level 6: the crusher" },
{ 0, 0, 0,       "level 7: dead simple" },
{ 0, 0, 0,       "level 8: tricks and traps" },
{ 0, 0, 0,       "level 9: the pit" },
{ 0, 0, 0,      "level 10: refueling base" },
{ 0, 0, 0,      "level 11: 'o' of destruction!" },
{ 0, 0, 0,      "level 12: the factory" },
{ 0, 0, 0,      "level 13: downtown" },
{ 0, 0, 0,      "level 14: the inmost dens" },
{ 0, 0, 0,      "level 15: industrial zone" },
{ 0, 0, 0,      "level 16: suburbs" },
{ 0, 0, 0,      "level 17: tenements" },
{ 0, 0, 0,      "level 18: the courtyard" },
{ 0, 0, 0,      "level 19: the citadel" },
{ 0, 0, 0,      "level 20: gotcha!" },
{ 0, 0, 0,      "level 21: nirvana" },
{ 0, 0, 0,      "level 22: the catacombs" },
{ 0, 0, 0,      "level 23: barrels o' fun" },
{ 0, 0, 0,      "level 24: the chasm" },
{ 0, 0, 0,      "level 25: bloodfalls" },
{ 0, 0, 0,      "level 26: the abandoned mines" },
{ 0, 0, 0,      "level 27: monster condo" },
{ 0, 0, 0,      "level 28: the spirit world" },
{ 0, 0, 0,      "level 29: the living end" },
{ 0, 0, 0,      "level 30: icon of sin" },
{ 0, 0, 0,      "level 31: wolfenstein" },
{ 0, 0, 0,      "level 32: grosse" },

{ 0, 0, 0,      "level 1: congo" },
{ 0, 0, 0,      "level 2: well of souls" },
{ 0, 0, 0,      "level 3: aztec" },
{ 0, 0, 0,      "level 4: caged" },
{ 0, 0, 0,      "level 5: ghost town" },
{ 0, 0, 0,      "level 6: baron's lair" },
{ 0, 0, 0,      "level 7: caughtyard" },
{ 0, 0, 0,      "level 8: realm" },
{ 0, 0, 0,      "level 9: abattoire" },
{ 0, 0, 0,     "level 10: onslaught" },
{ 0, 0, 0,     "level 11: hunted" },
{ 0, 0, 0,     "level 12: speed" },
{ 0, 0, 0,     "level 13: the crypt" },
{ 0, 0, 0,     "level 14: genesis" },
{ 0, 0, 0,     "level 15: the twilight" },
{ 0, 0, 0,     "level 16: the omen" },
{ 0, 0, 0,     "level 17: compound" },
{ 0, 0, 0,     "level 18: neurosphere" },
{ 0, 0, 0,     "level 19: nme" },
{ 0, 0, 0,     "level 20: the death domain" },
{ 0, 0, 0,     "level 21: slayer" },
{ 0, 0, 0,     "level 22: impossible mission" },
{ 0, 0, 0,     "level 23: tombstone" },
{ 0, 0, 0,     "level 24: the final frontier" },
{ 0, 0, 0,     "level 25: the temple of darkness" },
{ 0, 0, 0,     "level 26: bunker" },
{ 0, 0, 0,     "level 27: anti-christ" },
{ 0, 0, 0,     "level 28: the sewers" },
{ 0, 0, 0,     "level 29: odyssey of noises" },
{ 0, 0, 0,     "level 30: the gateway of hell" },
{ 0, 0, 0,     "level 31: cyberden" },
{ 0, 0, 0,     "level 32: go 2 it" },

{ 0, 0, 0,      "level 1: system control" },
{ 0, 0, 0,      "level 2: human bbq" },
{ 0, 0, 0,      "level 3: power control" },
{ 0, 0, 0,      "level 4: wormhole" },
{ 0, 0, 0,      "level 5: hanger" },
{ 0, 0, 0,      "level 6: open season" },
{ 0, 0, 0,      "level 7: prison" },
{ 0, 0, 0,      "level 8: metal" },
{ 0, 0, 0,      "level 9: stronghold" },
{ 0, 0, 0,     "level 10: redemption" },
{ 0, 0, 0,     "level 11: storage facility" },
{ 0, 0, 0,     "level 12: crater" },
{ 0, 0, 0,     "level 13: nukage processing" },
{ 0, 0, 0,     "level 14: steel works" },
{ 0, 0, 0,     "level 15: dead zone" },
{ 0, 0, 0,     "level 16: deepest reaches" },
{ 0, 0, 0,     "level 17: processing area" },
{ 0, 0, 0,     "level 18: mill" },
{ 0, 0, 0,     "level 19: shipping/respawning" },
{ 0, 0, 0,     "level 20: central processing" },
{ 0, 0, 0,     "level 21: administration center" },
{ 0, 0, 0,     "level 22: habitat" },
{ 0, 0, 0,     "level 23: lunar mining project" },
{ 0, 0, 0,     "level 24: quarry" },
{ 0, 0, 0,     "level 25: baron's den" },
{ 0, 0, 0,     "level 26: ballistyx" },
{ 0, 0, 0,     "level 27: mount pain" },
{ 0, 0, 0,     "level 28: heck" },
{ 0, 0, 0,     "level 29: river styx" },
{ 0, 0, 0,     "level 30: last call" },
{ 0, 0, 0,     "level 31: pharaoh" },
{ 0, 0, 0,     "level 32: caribbean" },

#endif

{ 0, 0, 0,  NULL }

};


/*
 ===================
 FindMapStats
 
 Finds or creats a mapStats_t structure for the given level.
 This can return NULL if the entire array is filled up, which may
 happen when people have an absurd number of downloaded levels.
 ===================
 */
mapStats_t *FindMapStats( int dataset, int episode, int map, boolean create ) {
	for ( int i = 0 ; i < playState.numMapStats ; i++ ) {
		mapStats_t *ms = &playState.mapStats[i];
		if ( ms->dataset == dataset && ms->episode == episode && ms->map == map ) {
			return ms;
		}
	}
	if ( playState.numMapStats == MAX_MAPS ) {
		// all full.
		return NULL;
	}
	
	if ( !create ) {
		return NULL;
	}
	mapStats_t *cms = &playState.mapStats[playState.numMapStats];
	cms->dataset = dataset;
	cms->episode = episode;
	cms->map = map;
	playState.numMapStats++;
	
	return cms;
}

/*
 ==================
 FindMapName
 
 episodes and maps are one base 
 ==================
 */
const char *FindMapName( int dataset, int episode, int map ) {
	for ( mapData_t *md = mapData ; md->name ; md++ ) {
		if ( md->dataset == dataset && md->episode == episode && md->map == map ) {
			return md->name;
		}
	}
	return "UNKNOWN MAP NAME";
}


/*
 ==================
 iphoneMapSelectMenu
 
 Skills are zero based:
 sk_baby=0,
 sk_easy,
 sk_medium,
 sk_hard,
 sk_nightmare=4
 
 episodes are one base
 
 ==================
 */
static const int MAP_ROW_HEIGHT = 52;
ibutton_t	btnSkills[4];	
int		dragVelocity;
ibutton_t	dragScroll;
mapData_t *selectedMap;
int		totalDrag;		// for determining if a release will activate the level
boolean iphoneMapSelectMenu( mapStart_t *map ) {
	static int prevDragY;
	
	if ( !dragScroll.x ) {
		// first initialization
		dragScroll.drawWidth = 480 - 80 - 64;
		dragScroll.drawHeight = 320;
		dragScroll.x = 64 + dragScroll.drawWidth / 2;
		dragScroll.y = 160;

		static char * skillNames[4] = {
			"iphone/skill_easy.tga",
			"iphone/skill_normal.tga",
			"iphone/skill_hard.tga",
			"iphone/skill_nightmare.tga" };	// not really "nightmare" skill since easy is "baby"
		for ( int i = 0 ; i < 4 ; i++ ) {
			SetButtonPics( &btnSkills[i], skillNames[i], "", 400, i*80 );
		}
	}
	
	// check for drag-scrolling
	if ( dragScroll.touch ) {
		if ( dragScroll.touch->y != prevDragY ) {
			dragVelocity = dragScroll.touch->y - prevDragY;
			prevDragY = dragScroll.touch->y;
			totalDrag += abs( dragVelocity );
		}
	}
	Cvar_SetValue( mapSelectY->name, mapSelectY->value - dragVelocity );
	
	// decay the dragVelocity
	for ( int i = 0 ; i < 2 ; i++ ) {
		if ( dragVelocity < 0 ) {
			dragVelocity++;
		} else if ( dragVelocity > 0 ) {
			dragVelocity--;
		}
	}
	
	int	iskill = (int)skill->value;
	if ( iskill < 0 ) {
		iskill = 0;
		Cvar_SetValue( skill->name, iskill );
	} else if ( iskill >= MAX_SKILLS ) {
		iskill = MAX_SKILLS-1;
		Cvar_SetValue( skill->name, iskill );
	}
	
	// snap back to bounds if dragging past end
	if ( mapSelectY->value < 0 ) {
		Cvar_SetValue( mapSelectY->name, 0 );
	}
	int	numMaps = 0;
	for ( mapData_t *map = mapData ; map->name != NULL ; map++ ) {
		numMaps++;
	}
	if ( mapSelectY->value > numMaps * MAP_ROW_HEIGHT - 320 ) {
		Cvar_SetValue( mapSelectY->name, numMaps * MAP_ROW_HEIGHT - 320 );
	}
		
	// scrolling display of levels
	int		y = -mapSelectY->value;
	mapData_t	*startMap = NULL;
	for ( mapData_t *map = mapData ; map->name != NULL ; map++ ) {
		if ( y > -64 && y < 320 ) {
			// find the mapStat_t for this map, if it has ever been started
			int	completionFlags = 0;
			mapStats_t *ms = FindMapStats( map->dataset, map->episode, map->map, false );
			if ( ms ) {
				completionFlags = ms->completionFlags[iskill];
			}
		
			// if we aren't already dragging, check for a touch on a map button
			if ( !dragScroll.touch ) {
				touch_t *touch = TouchInBounds( 120, y, 400-128, 48 );
				if ( touch ) {
					Sound_StartLocalSound( "iphone/bdown_01.wav" );	
					dragScroll.touch = touch;
					prevDragY = touch->y;
					touch->controlOwner = &dragScroll;
					selectedMap = map;
				}
			}
			
			// color background based on selected / entered / completed state
			if ( selectedMap == map ) {
				glColor4f( 1,1,1,1 );	// launch if released
			} else if ( completionFlags & MF_COMPLETED ) {
				glColor4f( 0.2, 0.5, 0.2, 1 );
			} else if ( completionFlags & MF_TRIED ) {
				glColor4f( 0.5, 0.2, 0.2, 1 );
			} else {
				glColor4f( 0.4, 0.4, 0.4, 1 );
			}
			
			// use -1 x to avoid a texture wrap seam
			PK_StretchTexture( PK_FindTexture( "iphone/long_string_box.tga" ),  110, y, 400-114, 48 );
			glColor4f( 1,1,1,1 );

			// draw the text
			float w = StringFontWidth( map->name );
			float fontScale = 0.75;
			if ( w > 360 ) {
				fontScale *= ( 360 / w );
			}
			iphoneDrawText( 120, y+32, fontScale, map->name );
			
			// add the awards
			if ( completionFlags & MF_KILLS ) {
				PK_DrawTexture( PK_FindTexture( "iphone/kills.tga" ),  80, y+4 );
			}
			if ( completionFlags & MF_TIME ) {
				PK_DrawTexture( PK_FindTexture( "iphone/par.tga" ),  40, y+4 );
			}
			if ( completionFlags & MF_SECRETS ) {
				PK_DrawTexture( PK_FindTexture( "iphone/secrets.tga" ),  0, y+4 );
			}
			
		}
		y += MAP_ROW_HEIGHT;
	}
	
	if ( numTouches == 0 ) {
		totalDrag = 0;
	}
		
	// draw the skill level	
	for ( int i = 0 ; i < 4 ; i++ ) {			
		if ( i == iskill ) {
			btnSkills[i].buttonFlags = 0;
		} else {
			btnSkills[i].buttonFlags = BF_DIMMED;
		}
		if ( HandleButton( &btnSkills[i] ) ) {
			Cvar_SetValue( skill->name, i );
		}
	}
	glColor4f( 1, 1, 1, 1 );
		
	// handle back button before checking for touches in the awards area
	if ( BackButton() ) {
		map->map = -1;
		return true;
	}
	
	// if we aren't already dragging, check for a touch anywhere outside the skill buttons
	if ( !dragScroll.touch ) {
		touch_t *touch = TouchInBounds( 0, 0, 400-128, 320 );
		if ( touch ) {
			dragScroll.touch = touch;
			prevDragY = touch->y;
			touch->controlOwner = &dragScroll;
			selectedMap = NULL;
			Sound_StartLocalSoundAtVolume( "iphone/controller_down_01_SILENCE.wav", touchClick->value );
		}
	} else {
		// if we dragged more than a few pixels, don't launch the level
		if ( totalDrag > 8 ) {
			selectedMap = NULL;
		}
		if ( !dragScroll.touch->down ) {
			// lifted finger
			dragScroll.touch = NULL;
			if ( selectedMap ) {
				Sound_StartLocalSound( "iphone/baction_01.wav" );	
				startMap = selectedMap;
				selectedMap = NULL;
			} else {
				Sound_StartLocalSoundAtVolume( "iphone/controller_up_01_SILENCE.wav", touchClick->value );
			}
		}
	}
	
	
	if ( !startMap ) {
		return false;
	}

	map->skill = iskill;
	map->episode = startMap->episode;
	map->map = startMap->map;
	map->dataset = startMap->dataset;

	return true;
}
