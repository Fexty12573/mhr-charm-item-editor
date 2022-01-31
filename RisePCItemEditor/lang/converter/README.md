# Names converter

Convert items and skill names into language CSV files.

## How to use

1. Please use the [RETool](https://github.com/mhvuze/MonsterHunterRiseModding/wiki/Tool-Collection#file-containers) to unpack the `.pak`.
2. Use the [REngine Text Tool](https://zenhax.com/viewtopic.php?t=13337) to export the `.msg` file into `.txt`. The `.msg` file for item names can be found at `re_chunk_000\natives\STM\data\System\ContentsIdSystem\Item\Normal\ItemName.msg.17` and the `.msg` file for skill names can be found at `re_chunk_000\natives\STM\data\Define\Player\Skill\PlEquipSkill\PlayerSkill_Name.msg.17`. Please export them into your the language you need. The example usage will looks like:

   ```
   REngine_Text-Tool\REngine_Text-Tool.exe re_chunk_000\natives\STM\data\Define\Player\Skill\PlEquipSkill\PlayerSkill_Name.msg.17 zhTW
   REngine_Text-Tool\REngine_Text-Tool.exe re_chunk_000\natives\STM\data\System\ContentsIdSystem\Item\Normal\ItemName.msg.17 zhTW
   ```
3. After getting the converted files, put them under `RisePCItemEditor/lang/source` with a different name. For example, `ItemName.msg.17.zhTW.txt`.
4. Run the converter:
   ```
   cd converter
   converter.exe zhTW ../source/ItemName.msg.17.zhTW.txt ../source/PlayerSkill_Name.msg.17.zhTW.txt
   ```
5. You will get the CSV now, please edit the `.ini` file in your language and use the new CSV with the charm editor. Note, the `.ini` file should be saved with different encoding for your language. For example, save as `Big-5` for zhTW.
