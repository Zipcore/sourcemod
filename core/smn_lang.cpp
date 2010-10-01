/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "PluginSys.h"
#include "Translator.h"
#include "LibrarySys.h"
#include "sm_stringutil.h"
#include "PlayerManager.h"

static cell_t sm_LoadTranslations(IPluginContext *pCtx, const cell_t *params)
{
	char *filename, *ext;
	char buffer[PLATFORM_MAX_PATH];
	CPlugin *pl = (CPlugin *)g_PluginSys.FindPluginByContext(pCtx->GetContext());

	pCtx->LocalToString(params[1], &filename);
	UTIL_Format(buffer, sizeof(buffer), "%s", filename);

	/* Make sure there is no extension */
	if ((ext = strstr(buffer, ".txt")) != NULL
		|| (ext = strstr(buffer, ".cfg")) != NULL)
	{
		/* Simple heuristic -- just see if it's at the end and terminate if so */
		if ((unsigned)(ext - buffer) == strlen(buffer) - 4)
		{
			*ext = '\0';
		}
	}

	pl->GetPhrases()->AddPhraseFile(buffer);

	return 1;
}

static cell_t sm_SetGlobalTransTarget(IPluginContext *pContext, const cell_t *params)
{
	g_SourceMod.SetGlobalTarget(params[1]);

	return 1;
}

static cell_t sm_GetClientLanguage(IPluginContext *pContext, const cell_t *params)
{
	CPlayer *player = g_Players.GetPlayerByIndex(params[1]);
	if (!player || !player->IsConnected())
	{
		return pContext->ThrowNativeError("Invalid client index %d", params[1]);
	}

	return g_Translator.GetClientLanguage(params[1]);
}

static cell_t sm_GetServerLanguage(IPluginContext *pContext, const cell_t *params)
{
	return g_Translator.GetServerLanguage();
}

static cell_t sm_GetLanguageCount(IPluginContext *pContext, const cell_t *params)
{
	return g_Translator.GetLanguageCount();
}

static cell_t sm_GetLanguageInfo(IPluginContext *pContext, const cell_t *params)
{
	const char *code;
	const char *name;

	if (!g_Translator.GetLanguageInfo(params[1], &code, &name))
	{
		return pContext->ThrowNativeError("Invalid language number %d", params[1]);
	}

	pContext->StringToLocalUTF8(params[2], params[3], code, NULL);
	pContext->StringToLocalUTF8(params[4], params[5], name, NULL);

	return 1;
}

REGISTER_NATIVES(langNatives)
{
	{"LoadTranslations",			sm_LoadTranslations},
	{"SetGlobalTransTarget",		sm_SetGlobalTransTarget},
	{"GetClientLanguage",			sm_GetClientLanguage},
	{"GetServerLanguage",			sm_GetServerLanguage},
	{"GetLanguageCount",			sm_GetLanguageCount},
	{"GetLanguageInfo",				sm_GetLanguageInfo},
	{NULL,							NULL},
};