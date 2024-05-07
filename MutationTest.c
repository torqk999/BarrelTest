#include <MutationTest.h>

void mutationTest(){

	/*
	
		GenePool myGenePool =
		GENE_POOL(3,
			MUTAGEN("BaseCharacter",
				PROP_INFO(name, char*),
				PROP_INFO(health, float),
				PROP_INFO(pos_X, float),
				PROP_INFO(pos_Y, float),
				PROP_INFO(pos_Z, float)
			),
			MUTAGEN("Warrior",
				PROP_INFO(stamina, float)
			),
			MUTAGEN("Mage",
				PROP_INFO(mana, float)
			)
		);

	*/

	Mutable myGenePool =
		GENE_POOL(3,
			MUTAGEN("BaseCharacter",
				PROP_INFO(name, char*),
				PROP_INFO(health, float),
				PROP_INFO(pos_X, float),
				PROP_INFO(pos_Y, float),
				PROP_INFO(pos_Z, float)
			),
			MUTAGEN("Warrior",
				PROP_INFO(stamina, float)
			),
			MUTAGEN("Mage",
				PROP_INFO(mana, float)
			)
		);
				
}

