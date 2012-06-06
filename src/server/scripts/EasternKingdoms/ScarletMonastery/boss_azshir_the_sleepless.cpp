/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2012 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You can't share Myth Project's sources! Only for personal use.
 */

#include "ScriptPCH.h"

#define SPELL_CALLOFTHEGRAVE            17831
#define SPELL_TERRIFY                   7399
#define SPELL_SOULSIPHON                7290

class boss_azshir_the_sleepless : public CreatureScript
{
public:
    boss_azshir_the_sleepless() : CreatureScript("boss_azshir_the_sleepless") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_azshir_the_sleeplessAI (creature);
    }

    struct boss_azshir_the_sleeplessAI : public ScriptedAI
    {
        boss_azshir_the_sleeplessAI(Creature* c) : ScriptedAI(c) { }

        uint32 SoulSiphon_Timer;
        uint32 CallOftheGrave_Timer;
        uint32 Terrify_Timer;

        void Reset()
        {
            SoulSiphon_Timer = 1;
            CallOftheGrave_Timer = 30000;
            Terrify_Timer = 20000;
        }

        void EnterCombat(Unit* /*pWho*/) { }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            //If we are <50% hp cast Soul Siphon rank 1
            if(!HealthAbovePct(50) && !me->IsNonMeleeSpellCasted(false))
            {
                //SoulSiphon_Timer
                if(SoulSiphon_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_SOULSIPHON);
                    return;

                    //SoulSiphon_Timer = 20000;
                } else SoulSiphon_Timer -= diff;
            }

            //CallOfTheGrave_Timer
            if(CallOftheGrave_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CALLOFTHEGRAVE);
                CallOftheGrave_Timer = 30000;
            } else CallOftheGrave_Timer -= diff;

            //Terrify_Timer
            if(Terrify_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_TERRIFY);
                Terrify_Timer = 20000;
            } else Terrify_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_azshir_the_sleepless()
{
    new boss_azshir_the_sleepless;
}