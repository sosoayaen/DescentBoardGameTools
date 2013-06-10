package.path = ".\\script\\?.lua;" .. package.path
-- ��ͼ����˵��
MapTypeTbl =
{
	"��ͼģ��",
	"��",
	"�ϰ���",
	"һ����Ʒ",
	"����",
	"��ͨ����",
	"��Ӣ����",
	"Ӣ��",
}

-- ��ͼ���͵����ֵ������еĶ�Ӧ���͵�ֵ
MapTypeName2TypeValue =
{
	["corridor"] = 0;
	["room"] = 0;
	["door"] = 1;
	["obstacle"] = 2;
	["object"] = 3;
	["treasure"] = 4;
	["normal monster"] = 5;
	["master monster"] = 6;
	["hero"] = 7;
}

-- ��ͼͼƬ��Ӧ��λ��
ImageFilePath =
{
	Heroes = 
	{
		{id = 3000, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "andira_runehand.png", displayName = "Andira Runehand"},
		{id = 3001, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "arvel_worldwalker.png", displayName = "Arvel Worldwalker"}, 
		{id = 3002, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "aurim.png", displayName = "Aurim"}, 
		{id = 3003, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "battlemage_jaes.png", displayName = "Battlemage Jaes"}, 
		{id = 3004, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "bogran_the_shadow.png", displayName = "Bogran the Shadow"}, 
		{id = 3005, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "brother_gherinn.png", displayName = "Brother Gherinn"}, 
		{id = 3006, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "brother_glyr.png", displayName = "Brother Glyr"}, 
		{id = 3007, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "challara.png", displayName = "Challara"}, 
		{id = 3008, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "corbin.png", displayName = "Corbin"}, 
		{id = 3009, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "eliam.png", displayName = "Eliam"}, 
		{id = 3010, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "grey_ker.png", displayName = "Grey Ker"}, 
		{id = 3011, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "hugo_the_glorious.png", displayName = "Hugo the Glorious"}, 
		{id = 3012, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "ispher.png", displayName = "Ispher"}, 
		{id = 3013, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "jonas_the_kind.png", displayName = "Jonas the Kind"}, 
		{id = 3014, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "karnon.png", displayName = "Karnon"}, 
		{id = 3015, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "kirga.png", displayName = "Kirga"}, 
		{id = 3016, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "krutzbeck.png", displayName = "Krutzbeck"}, 
		{id = 3017, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "landrec_the_wise.png", displayName = "Landrec the Wise"}, 
		{id = 3018, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "laughin_buldar.png", displayName = "Laughin Buldar"}, 
		{id = 3019, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "laurel_of_bloodwood.png", displayName = "Laurel of Bloodwood"}, 
		{id = 3020, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "lindel.png", displayName = "Lindel"}, 
		{id = 3021, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "lord_hawthorne.png", displayName = "Lord Hawthorne"}, 
		{id = 3022, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "lyssa.png", displayName = "Lyssa"}, 
		{id = 3023, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "mad_carthos.png", displayName = "Mad Carthos"}, 
		{id = 3024, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "mordrog.png", displayName = "Mordrog"}, 
		{id = 3025, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "nanok_of_the_blade.png", displayName = "Nanok of the Blade"}, 
		{id = 3026, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "nara_the_fang.png", displayName = "Nara the Fang"}, 
		{id = 3027, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "okaluk_and_rakash.png", displayName = "Okaluk and Rakash"}, 
		{id = 3028, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "one_fist.png", displayName = "One Fist"}, 
		{id = 3029, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "red_scorpion.png", displayName = "Red Scorpion"}, 
		{id = 3030, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "ronan_of_the_wild.png", displayName = "Ronan of the Wild"}, 
		{id = 3031, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "runemaster_thorn.png", displayName = "Runemaster Thorn"}, 
		{id = 3032, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "runewitch_astarra.png", displayName = "Runewitch Astarra"}, 
		{id = 3033, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "sahla.png", displayName = "Sahla"}, 
		{id = 3034, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "shiver.png", displayName = "Shiver"}, 
		{id = 3035, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "silhouette.png", displayName = "Silhouette"}, 
		{id = 3036, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "sir_valadir.png", displayName = "Sir Valadir"}, 
		{id = 3037, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "spiritspeaker_mok.png", displayName = "Spiritspeaker Mok"}, 
		{id = 3038, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "steelhorns.png", displayName = "Steelhorns"}, 
		{id = 3039, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "tahlia.png", displayName = "Tahlia"}, 
		{id = 3040, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "tatianna.png", displayName = "Tatianna"}, 
		{id = 3041, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "tetherys.png", displayName = "Tetherys"}, 
		{id = 3042, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "tobin_farslayer.png", displayName = "Tobin Farslayer"}, 
		{id = 3043, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "trenloe_the_strong.png", displayName = "Trenloe the Strong"}, 
		{id = 3044, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "truthseer_kel.png", displayName = "Truthseer Kel"}, 
		{id = 3045, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "varikas_the_dead.png", displayName = "Varikas the Dead"}, 
		{id = 3046, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "vyrah_the_falconer.png", displayName = "Vyrah the Falconer"}, 
		{id = 3047, type = "hero", width = 1, height = 1, path = "images/heroes/", name = "zyla.png", displayName = "Zyla"}, 

	},
	Tiles = 
	{
		{id = 1, type = "corridor", width = 2, height = 2, path = "images/tiles/", name = "corridor_2x2.png", displayName = "���� 2��2"},
		{id = 2, type = "corridor", width = 4, height = 2, path = "images/tiles/", name = "corridor_4x2.png", displayName = "���� 4��2"},
		{id = 3, type = "corridor", width = 6, height = 2, path = "images/tiles/", name = "corridor_6x2.png", displayName = "���� 6��2"},
		{id = 4, type = "corridor", width = 6, height = 2, path = "images/tiles/", name = "corridor_6x2_corrupted.png", displayName = "�������ٻ��� 6��2"},
		{id = 5, type = "corridor", width = 1, height = 2, path = "images/tiles/", name = "corridor_end.png", displayName = "��ͼĩ��"},
		{id = 6, type = "corridor", width = 1, height = 2, path = "images/tiles/", name = "corridor_end_corrupted.png", displayName = "��ͼĩ�ˣ��ٻ���"},
		{id = 7, type = "corridor", width = 4, height = 4, path = "images/tiles/", name = "corridor_intersection.png", displayName = "����ʮ����"},
		{id = 8, type = "corridor", width = 4, height = 4, path = "images/tiles/", name = "corridor_intersection_corrupted.png", displayName = "����ʮ���ͣ��ٻ���"},
		{id = 9, type = "corridor", width = 4, height = 4, path = "images/tiles/", name = "corridor_L-connector.png", displayName = "����L��"},
		{id = 10, type = "corridor", width = 4, height = 4, path = "images/tiles/", name = "corridor_t-junction.png", displayName = "����T��"},
		{id = 11, type = "room", width = 4, height = 4, path = "images/tiles/", name = "room_4x4.png", displayName = "���� 4��4"},
		{id = 12, type = "room", width = 4, height = 4, path = "images/tiles/", name = "room_4x4_corrupted.png", displayName = "���䣨�ٻ��� 4��4"},
		{id = 13, type = "room", width = 4, height = 6, path = "images/tiles/", name = "room_4x6.png", displayName = "���� 4��6"},
		{id = 14, type = "room", width = 4, height = 6, path = "images/tiles/", name = "room_4x6_muddy.png", displayName = "���䣨��Ţ�� 4��6"},
		{id = 15, type = "room", width = 4, height = 6, path = "images/tiles/", name = "room_6x6.png", displayName = "���� 6��6"},
	},
	Monsters =
	{
		{id = 1000, type = "normal monster", width = 2, height = 2, path = "images/monsters/normal/", name = "bane_spider.png", displayName = "��֩��"},
		{id = 1001, type = "normal monster", width = 1, height = 1, path = "images/monsters/normal/", name = "beastman.png", displayName = "����"},
		{id = 1002, type = "normal monster", width = 2, height = 1, path = "images/monsters/normal/", name = "blood_ape.png", displayName = "ѪԳ"},
		{id = 1003, type = "normal monster", width = 2, height = 2, path = "images/monsters/normal/", name = "chaos_beast.png", displayName = "���ҹ���"},
		{id = 1004, type = "normal monster", width = 1, height = 1, path = "images/monsters/normal/", name = "dark_priest.png", displayName = "�ڰ���ʿ"},
		{id = 1005, type = "normal monster", width = 1, height = 1, path = "images/monsters/normal/", name = "deep_elf.png", displayName = "����"},
		{id = 1006, type = "normal monster", width = 2, height = 2, path = "images/monsters/normal/", name = "demon.png", displayName = "��ħ"},
		{id = 1007, type = "normal monster", width = 2, height = 3, path = "images/monsters/normal/", name = "dragon.png", displayName = "��"},
		{id = 1008, type = "normal monster", width = 1, height = 1, path = "images/monsters/normal/", name = "ferrox.png", displayName = "������"},
		{id = 1009, type = "normal monster", width = 2, height = 2, path = "images/monsters/normal/", name = "giant.png", displayName = "����"},
		{id = 1010, type = "normal monster", width = 2, height = 2, path = "images/monsters/normal/", name = "golem.png", displayName = "ʯħ��"},
		{id = 1011, type = "normal monster", width = 2, height = 1, path = "images/monsters/normal/", name = "hellhound.png", displayName = "����Ȯ"},
		{id = 1012, type = "normal monster", width = 1, height = 1, path = "images/monsters/normal/", name = "kobold.png", displayName = "�ؾ�"},
		{id = 1013, type = "normal monster", width = 2, height = 2, path = "images/monsters/normal/", name = "manticore.png", displayName = "Ыʨ"},
		{id = 1014, type = "normal monster", width = 2, height = 2, path = "images/monsters/normal/", name = "naga.png", displayName = "����"},
		{id = 1015, type = "normal monster", width = 2, height = 2, path = "images/monsters/normal/", name = "ogre.png", displayName = "��ħ"},
		{id = 1016, type = "normal monster", width = 1, height = 1, path = "images/monsters/normal/", name = "razorwing.png", displayName = "������"},
		{id = 1017, type = "normal monster", width = 1, height = 1, path = "images/monsters/normal/", name = "skeleton.png", displayName = "����"},
		{id = 1018, type = "normal monster", width = 1, height = 1, path = "images/monsters/normal/", name = "sorcerer.png", displayName = "��ʦ"},
		{id = 1019, type = "normal monster", width = 2, height = 2, path = "images/monsters/normal/", name = "troll.png", displayName = "������"},
		{id = 1020, type = "normal monster", width = 1, height = 1, path = "images/monsters/normal/", name = "the_haunt.png", displayName = "����"},
	},
	Masters =
	{
		{id = 2000, type = "master monster", width = 2, height = 2, path = "images/monsters/master/", name = "bane_spider_master.png", displayName = "�ֲ�֩��"},
		{id = 2001, type = "master monster", width = 1, height = 1, path = "images/monsters/master/", name = "beastman_master.png", displayName = "Ѫ������"},
		{id = 2002, type = "master monster", width = 2, height = 1, path = "images/monsters/master/", name = "blood_ape_master.png", displayName = "Ѫ��Գ��"},
		{id = 2003, type = "master monster", width = 2, height = 2, path = "images/monsters/master/", name = "chaos_beast_master.png", displayName = "�����޳�"},
		{id = 2004, type = "master monster", width = 1, height = 1, path = "images/monsters/master/", name = "dark_priest_master.png", displayName = "�ڰ��̳�"},
		{id = 2005, type = "master monster", width = 1, height = 1, path = "images/monsters/master/", name = "deep_elf_master.png", displayName = "�����峤"},
		{id = 2006, type = "master monster", width = 2, height = 2, path = "images/monsters/master/", name = "demon_master.png", displayName = "��Ӣ��ħ"},
		{id = 2007, type = "master monster", width = 2, height = 3, path = "images/monsters/master/", name = "dragon_master.png", displayName = "����"},
		{id = 2008, type = "master monster", width = 1, height = 1, path = "images/monsters/master/", name = "ferrox_master.png", displayName = "��Ӣ������"},
		{id = 2009, type = "master monster", width = 2, height = 2, path = "images/monsters/master/", name = "giant_master.png", displayName = "�����峤"},
		{id = 2010, type = "master monster", width = 2, height = 2, path = "images/monsters/master/", name = "golem_master.png", displayName = "��ħ��"},
		{id = 2011, type = "master monster", width = 2, height = 1, path = "images/monsters/master/", name = "hellhound_master.png", displayName = "����Ȯͷ��"},
		{id = 2012, type = "master monster", width = 1, height = 1, path = "images/monsters/master/", name = "kobold_master.png", displayName = "�ؾ��峤"},
		{id = 2013, type = "master monster", width = 2, height = 2, path = "images/monsters/master/", name = "manticore_master.png", displayName = "Ыʨͷ��"},
		{id = 2014, type = "master monster", width = 2, height = 2, path = "images/monsters/master/", name = "naga_master.png", displayName = "�����峤"},
		{id = 2015, type = "master monster", width = 2, height = 2, path = "images/monsters/master/", name = "ogre_master.png", displayName = "��Ӣ��ħ"},
		{id = 2016, type = "master monster", width = 1, height = 1, path = "images/monsters/master/", name = "razorwing_master.png", displayName = "ħ��"},
		{id = 2017, type = "master monster", width = 1, height = 1, path = "images/monsters/master/", name = "skeleton_master.png", displayName = "��Ӣ����"},
		{id = 2018, type = "master monster", width = 1, height = 1, path = "images/monsters/master/", name = "sorcerer_master.png", displayName = "��Ӣ����"},
		{id = 2019, type = "master monster", width = 2, height = 2, path = "images/monsters/master/", name = "troll_master.png", displayName = "������ͷ��"},

	},
	Objects =
	{
		{id = 5000, type = "object", width = 1, height = 1, path = "images/objects/", name = "compass_rose.png", displayName = "�����־"},
		{id = 5001, type = "object", width = 1, height = 1, path = "images/objects/", name = "encounter.png", displayName = "�¼���־"},
		{id = 5002, type = "object", width = 1, height = 1, path = "images/objects/", name = "glyph_active.png", displayName = "������-����"},
		{id = 5003, type = "object", width = 1, height = 1, path = "images/objects/", name = "glyph_inactive.png", displayName = "������-ʧЧ"},
		{id = 5004, type = "object", width = 1, height = 1, path = "images/objects/", name = "glyph_of_power.png", displayName = "����������"},
		{id = 5005, type = "object", width = 1, height = 1, path = "images/objects/", name = "glyph_of_summoning.png", displayName = "��紫����"},
		{id = 5006, type = "object", width = 1, height = 1, path = "images/objects/", name = "glyph_sundered.png", displayName = "���Ѵ�����"},
		{id = 5007, type = "object", width = 1, height = 1, path = "images/objects/", name = "rune_blue.png", displayName = "����-��ɫ"},
		{id = 5008, type = "object", width = 1, height = 1, path = "images/objects/", name = "rune_red.png", displayName = "����-��ɫ"},
		{id = 5009, type = "object", width = 1, height = 1, path = "images/objects/", name = "rune_yellow.png", displayName = "����-��ɫ"},
		{id = 5010, type = "object", width = 1, height = 1, path = "images/objects/", name = "stairs_blue.png", displayName = "¥��-��ɫ"},
		{id = 5011, type = "object", width = 1, height = 1, path = "images/objects/", name = "stairs_green.png", displayName = "¥��-��ɫ"},
		{id = 5012, type = "object", width = 1, height = 1, path = "images/objects/", name = "stairs_grey.png", displayName = "¥��-��ɫ"},
		{id = 5013, type = "object", width = 1, height = 1, path = "images/objects/", name = "stairs_red.png", displayName = "¥��-��ɫ"},
	},
	Obstacles =
	{
		{id = 6000, type = "obstacle", width = 1, height = 1, path = "images/obstacles/", name = "corruption_1x1.png", displayName = "���·�� 1��1"},
		{id = 6001, type = "obstacle", width = 2, height = 1, path = "images/obstacles/", name = "corruption_1x2.png", displayName = "���·�� 1��2"},
		{id = 6002, type = "obstacle", width = 3, height = 1, path = "images/obstacles/", name = "corruption_1x3.png", displayName = "���·�� 1��3"},
		{id = 6003, type = "obstacle", width = 1, height = 1, path = "images/obstacles/", name = "lava_1x1.png", displayName = "���� 1��1"},
		{id = 6004, type = "obstacle", width = 2, height = 1, path = "images/obstacles/", name = "lava_1x2.png", displayName = "���� 1��2"},
		{id = 6005, type = "obstacle", width = 3, height = 1, path = "images/obstacles/", name = "lava_1x3.png", displayName = "���� 1��3"},
		{id = 6006, type = "obstacle", width = 1, height = 1, path = "images/obstacles/", name = "mud_1x1.png", displayName = "�ཬ 1��1"},
		{id = 6007, type = "obstacle", width = 2, height = 1, path = "images/obstacles/", name = "mud_1x2.png", displayName = "�ཬ 1��2"},
		{id = 6008, type = "obstacle", width = 3, height = 1, path = "images/obstacles/", name = "mud_1x3.png", displayName = "�ཬ 1��3"},
		{id = 6009, type = "obstacle", width = 1, height = 1, path = "images/obstacles/", name = "pit_1x1.png", displayName = "���� 1��1"},
		{id = 6010, type = "obstacle", width = 2, height = 1, path = "images/obstacles/", name = "pit_1x2.png", displayName = "���� 1��2"},
		{id = 6012, type = "obstacle", width = 3, height = 1, path = "images/obstacles/", name = "pit_1x3.png", displayName = "���� 1��3"},
		{id = 6013, type = "obstacle", width = 4, height = 4, path = "images/obstacles/", name = "pit_4x4.png", displayName = "���� 4��4"},
		{id = 6014, type = "obstacle", width = 1, height = 1, path = "images/obstacles/", name = "rubble_1x1.png", displayName = "������ 1��1"},
		{id = 6015, type = "obstacle", width = 2, height = 1, path = "images/obstacles/", name = "rubble_1x2.png", displayName = "������ 1��2"},
		{id = 6016, type = "obstacle", width = 3, height = 1, path = "images/obstacles/", name = "rubble_1x3.png", displayName = "������ 1��3"},
		{id = 6017, type = "obstacle", width = 1, height = 1, path = "images/obstacles/", name = "water_1x1.png", displayName = "ˮ̶ 1��1"},
		{id = 6018, type = "obstacle", width = 2, height = 1, path = "images/obstacles/", name = "water_1x2.png", displayName = "ˮ̶ 1��2"},
		{id = 6019, type = "obstacle", width = 3, height = 1, path = "images/obstacles/", name = "water_1x3.png", displayName = "ˮ̶ 1��3"},
		{id = 6020, type = "obstacle", width = 1, height = 1, path = "images/obstacles/", name = "Web.png", displayName = "����"},
	},
	Treasure =
	{
		{id = 7000, type = "treasure", width = 1, height = 1, path = "images/treasure/", name = "chest_copper.png", displayName = "ͭ����"},
		{id = 7001, type = "treasure", width = 1, height = 1, path = "images/treasure/", name = "chest_silver.png", displayName = "������"},
		{id = 7002, type = "treasure", width = 1, height = 1, path = "images/treasure/", name = "chest_gold.png", displayName = "����"},
		{id = 7003, type = "treasure", width = 1, height = 1, path = "images/treasure/", name = "pile_of_coins.png", displayName = "Ǯ��"},
		{id = 7004, type = "treasure", width = 1, height = 1, path = "images/treasure/", name = "potion_of_healing.png", displayName = "����ҩˮ"},
		{id = 7005, type = "treasure", width = 1, height = 1, path = "images/treasure/", name = "potion_of_vitality.png", displayName = "����ҩˮ"},
	},
	Doors =
	{
		{id = 4000, type = "door", width = 4, height = 2, path = "images/doors/", name = "door_normal.png", displayName = "��"},
		{id = 4001, type = "door", width = 4, height = 2, path = "images/doors/", name = "door_blue_rune_locked.png", displayName = "�ţ���ɫ��������"},
		{id = 4002, type = "door", width = 4, height = 2, path = "images/doors/", name = "door_red_rune_locked.png", displayName = "�ţ���ɫ��������"},
		{id = 4003, type = "door", width = 4, height = 2, path = "images/doors/", name = "door_yellow_rune_locked.png", displayName = "�ţ���ɫ��������"},
	},
	Dices =
	{
		
	},
}

require "lom"

local lxp = lxp;
local codeconvert = require "codeconvert"
-- ���õ�ͼ�ļ���ȡ
function ReadMapFile(fileName)
	local xmlFile = io.open(fileName, "r");
	local xml;
	if xmlFile then
		xml = xmlFile:read("*all");
		xmlFile:close();
	end

	xml = codeconvert.GBK2UTF8(xml);
	local tbl = lxp.lom.parse(xml);
	
	-- ���ر�
	local result = {};

	for mk, mtype in pairs(tbl) do
		if type(mtype) == 'table' and mk ~= 'attr' then
			local itemTbl = {};	-- һ�� item ������
			for ik, item in pairs(mtype) do
				if type(item) == 'table' then
					for ak, attr in pairs(item) do
						if type(attr) == 'table' then -- item.attr
							local MapElement = {};
							for pk, property in pairs(attr) do
								if type(pk) ~= 'number' then
--	 								print(string.format("%s=%s", pk, property));
									MapElement[pk] = codeconvert.UTF82GBK(property);	-- �ѵ�ͼԪ�����ӵ���ǰͼ����
								end
							end
							table.insert(itemTbl, MapElement);	-- ����һ��ͼ�㵽ͼ���б���
--	 						print(string.rep('-', 30));
						end
					end
				else
--	 				print("itemTbl's length:", #itemTbl);
					result[item] = itemTbl;
--	 				print(item);
				end
			end
--	 		print(string.rep('=', 30));
		end
	end
	
	-- ��ͼһ������
	result.Players = tbl.attr.Players or 2;
	result.version = tbl.attr.version or 'JITD';
	result.ThreatToken = tbl.attr.ThreatToken or 0;
	result.ConquestToken = tbl.attr.ConquestToken or 0;
	
	return result;
end

-- �������ݴ���ģ��
local DataHandle = require "DataHandle"

HandleMapObjectDefaultData = function(nID, nPlayers)
	nPlayers = nPlayers or 2;
	nID = tostring(nID);
	
	return DataHandle.GetDataTblFromID(nID, nPlayers);
end

