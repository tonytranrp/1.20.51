target_sources(Nuvola PRIVATE
	"IPatch.cpp"
	"PatchManager.cpp"
)

add_subdirectory(Actor)
add_subdirectory(BlockSource)
add_subdirectory(ClientInstance)
add_subdirectory(ContainerScreenController)
add_subdirectory(D3D12)
add_subdirectory(Dimension)
add_subdirectory(DXGI)
add_subdirectory(Global)
add_subdirectory(ItemUseSlowdownSystemImpl)
add_subdirectory(Level)
add_subdirectory(LoopbackPacketSender)
add_subdirectory(MobJumpFromGroundSystemImpl)
add_subdirectory(PacketHandler)
add_subdirectory(RakNet)
add_subdirectory(ScreenView)
add_subdirectory(GameMode)
# add_subdirectory(SurvivalMode)

