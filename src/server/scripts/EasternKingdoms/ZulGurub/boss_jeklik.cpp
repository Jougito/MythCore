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
#include "zulgurub.h"

#define SAY_AGGRO                   -1309002
#define SAY_RAIN_FIRE               -1309003
#define SAY_DEATH                   -1309004

#define SPELL_CHARGE              22911
#define SPELL_SONICBURST          23918
#define SPELL_SCREECH             6605
#define SPELL_SHADOW_WORD_PAIN    23952
#define SPELL_MIND_FLAY           23953
#define SPELL_CHAIN_MIND_FLAY     26044                     //Right ID unknown. So disabled
#define SPELL_GREATERHEAL         23954
#define SPELL_BAT_FORM            23966

// Batriders Spell

#define SPELL_BOMB                40332                     //Wrong ID but Magmadars bomb is not working...

class boss_jeklik : public CreatureScript
{
    public:
        boss_jeklik() : CreatureScript("boss_jeklik") { }

        struct boss_jeklikAI : public ScriptedAI
        {
            boss_jeklikAI(Creature* c) : ScriptedAI(c)
            {
                pInstance = c->GetInstanceScript();
            }

            InstanceScript *pInstance;

            uint32 Charge_Timer;
            uint32 SonicBurst_Timer;
            uint32 Screech_Timer;
            uint32 SpawnBats_Timer;
            uint32 ShadowWordPain_Timer;
            uint32 MindFlay_Timer;
            uint32 ChainMindFlay_Timer;
            uint32 GreaterHeal_Timer;
            uint32 SpawnFlyingBats_Timer;

            bool PhaseTwo;

            void Reset()
            {
                Charge_Timer = 20000;
                SonicBurst_Timer = 8000;
                Screech_Timer = 13000;
                SpawnBats_Timer = 60000;
                ShadowWordPain_Timer = 6000;
                MindFlay_Timer = 11000;
                ChainMindFlay_Timer = 26000;
                GreaterHeal_Timer = 50000;
                SpawnFlyingBats_Timer = 10000;

                PhaseTwo = false;
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoScriptText(SAY_AGGRO, me);
                DoCast(me, SPELL_BAT_FORM);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                DoScriptText(SAY_DEATH, me);

                if(pInstance)
                    pInstance->SetData(DATA_JEKLIK, DONE);
            }

            void UpdateAI(const uint32 diff)
            {
                if(!UpdateVictim())
                    return;

                if(me->getVictim() && me->isAlive())
                {
                    if(HealthAbovePct(50))
                    {
                        if(Charge_Timer <= diff)
                        {
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            {
                                DoCast(target, SPELL_CHARGE);
                                AttackStart(target);
                            }

                            Charge_Timer = 15000 + rand()%15000;
                        } else Charge_Timer -= diff;

                        if(SonicBurst_Timer <= diff)
                        {
                            DoCast(me->getVictim(), SPELL_SONICBURST);
                            SonicBurst_Timer = 8000 + rand()%5000;
                        } else SonicBurst_Timer -= diff;

                        if(Screech_Timer <= diff)
                        {
                            DoCast(me->getVictim(), SPELL_SCREECH);
                            Screech_Timer = 18000 + rand()%8000;
                        } else Screech_Timer -= diff;

                        if(SpawnBats_Timer <= diff)
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0);

                            Creature* Bat = NULL;
                            Bat = me->SummonCreature(11368, -12291.6220f, -1380.2640f, 144.8304f, 5.483f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if(target && Bat) Bat ->AI()->AttackStart(target);

                            Bat = me->SummonCreature(11368, -12289.6220f, -1380.2640f, 144.8304f, 5.483f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if(target && Bat) Bat ->AI()->AttackStart(target);

                            Bat = me->SummonCreature(11368, -12293.6220f, -1380.2640f, 144.8304f, 5.483f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if(target && Bat) Bat ->AI()->AttackStart(target);

                            Bat = me->SummonCreature(11368, -12291.6220f, -1380.2640f, 144.8304f, 5.483f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if(target && Bat) Bat ->AI()->AttackStart(target);

                            Bat = me->SummonCreature(11368, -12289.6220f, -1380.2640f, 144.8304f, 5.483f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if(target && Bat) Bat ->AI()->AttackStart(target);
                            Bat = me->SummonCreature(11368, -12293.6220f, -1380.2640f, 144.8304f, 5.483f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                            if(target && Bat) Bat ->AI()->AttackStart(target);

                            SpawnBats_Timer = 60000;
                        } else SpawnBats_Timer -= diff;
                    }
                    else
                    {
                        if(PhaseTwo)
                        {
                            if(PhaseTwo && ShadowWordPain_Timer <= diff)
                            {
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                {
                                    DoCast(target, SPELL_SHADOW_WORD_PAIN);
                                    ShadowWordPain_Timer = 12000 + rand()%6000;
                                }
                            }ShadowWordPain_Timer -=diff;

                            if(MindFlay_Timer <= diff)
                            {
                                DoCast(me->getVictim(), SPELL_MIND_FLAY);
                                MindFlay_Timer = 16000;
                            }MindFlay_Timer -=diff;

                            if(ChainMindFlay_Timer <= diff)
                            {
                                me->InterruptNonMeleeSpells(false);
                                DoCast(me->getVictim(), SPELL_CHAIN_MIND_FLAY);
                                ChainMindFlay_Timer = 15000 + rand()%15000;
                            }ChainMindFlay_Timer -=diff;

                            if(GreaterHeal_Timer <= diff)
                            {
                                me->InterruptNonMeleeSpells(false);
                                DoCast(me, SPELL_GREATERHEAL);
                                GreaterHeal_Timer = 25000 + rand()%10000;
                            }GreaterHeal_Timer -=diff;

                            if(SpawnFlyingBats_Timer <= diff)
                            {
                                Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                                if(!target)
                                    return;

                                Creature* FlyingBat = me->SummonCreature(14965, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ()+15, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                                if(FlyingBat)
                                    FlyingBat->AI()->AttackStart(target);

                                SpawnFlyingBats_Timer = 10000 + rand()%5000;
                            } else SpawnFlyingBats_Timer -=diff;
                        }
                        else
                        {
                            me->SetDisplayId(15219);
                            DoResetThreat();
                            PhaseTwo = true;
                        }
                    }

                    DoMeleeAttackIfReady();
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_jeklikAI(creature);
        }
};

//Flying Bat
class mob_batrider : public CreatureScript
{
    public:
        mob_batrider() : CreatureScript("mob_batrider") { }

        struct mob_batriderAI : public ScriptedAI
        {
            mob_batriderAI(Creature* c) : ScriptedAI(c)
            {
                pInstance = c->GetInstanceScript();
            }

            InstanceScript *pInstance;

            uint32 Bomb_Timer;
            uint32 Check_Timer;

            void Reset()
            {
                Bomb_Timer = 2000;
                Check_Timer = 1000;

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void EnterCombat(Unit* /*pWho*/) { }

            void UpdateAI (const uint32 diff)
            {
                if(!UpdateVictim())
                    return;

                //Bomb_Timer
                if(Bomb_Timer <= diff)
                {
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    {
                        DoCast(target, SPELL_BOMB);
                        Bomb_Timer = 5000;
                    }
                } else Bomb_Timer -= diff;

                //Check_Timer
                if(Check_Timer <= diff)
                {
                    if(pInstance)
                    {
                        if(pInstance->GetData(DATA_JEKLIK) == DONE)
                        {
                            me->setDeathState(JUST_DIED);
                            me->RemoveCorpse();
                            return;
                        }
                    }

                    Check_Timer = 1000;
                } else Check_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_batriderAI(creature);
        }
};

void AddSC_boss_jeklik()
{
    new boss_jeklik;
    new mob_batrider;
}

