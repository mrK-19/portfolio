DragonQuest AI  -- How to use --

1. qlearning.c
This code is for reproducing Q-learning.
You can execute this code and check how the player defeats enemy.
The function "learning" let AI learn by updating Q-value of each state and action.
The function "demo" let AI battle Dragonlord, the boss of DragonQuest I.

2. qlearning_plus.c
This code is for clarifying the enemy state balance.
I proved that player can't defeat enemy with certain HP and Attack.
Therefore, the difficulty can be adjusted by changing enemy HP and Attack within the range of limitation.
You can also check the limitation border by executing this code.
