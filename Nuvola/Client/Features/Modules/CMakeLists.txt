target_sources(Nuvola PRIVATE
	"BlockTypeListSetting.cpp"
	"Category.cpp"
	"EnchantWeightSetting.cpp"
	"EntityTypeListSetting.cpp"
	"GroupSetting.cpp"
	"Module.cpp"
	"ModuleManager.cpp"
)
add_subdirectory(Combat)
add_subdirectory(Misc)
add_subdirectory(Motion)
add_subdirectory(Player)
add_subdirectory(Render)