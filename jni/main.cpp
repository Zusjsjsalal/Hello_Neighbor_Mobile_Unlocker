#include <And64InlineHook/And64InlineHook.hpp>
#include <dlfcn.h>
#include <thread>
bool (*old_AGetInappState)(...);
bool (*old_ASetInappState)(...);
bool AGetInappState(void *instance){
    if (!old_AGetInappState(instance));
	old_ASetInappState(instance, true);
	return old_AGetInappState(instance);
}
bool ASetInappState(void *instance, bool a){
    return old_ASetInappState(instance, true);
}
bool (*old_UGetInappState)(...);
bool (*old_USetInappState)(...);
bool UGetInappState(void *instance){
    if (!old_UGetInappState(instance));
	old_USetInappState(instance, true);
	return old_UGetInappState(instance);
}
bool USetInappState(void *instance, bool a){
    return old_USetInappState(instance, true);
}
void main_geokar2006_thread(){
	void *lib_data = nullptr;
    while (!lib_data) {
        lib_data = dlopen("libUE4.so", RTLD_LAZY);
    }
    A64HookFunction((void *)dlsym(lib_data, "_ZN13URecoveryData13GetInappStateEv"), (void *)UGetInappState, (void **)&old_UGetInappState);
	A64HookFunction((void *)dlsym(lib_data, "_ZN13URecoveryData13SetInappStateEb"), (void *)USetInappState, (void **)&old_USetInappState);
	A64HookFunction((void *)dlsym(lib_data, "_ZN28AHelloNeighborRebornGameMode13GetInappStateEv"), (void *)AGetInappState, (void **)&old_AGetInappState);
	A64HookFunction((void *)dlsym(lib_data, "_ZN28AHelloNeighborRebornGameMode13SetInappStateEb"), (void *)ASetInappState, (void **)&old_ASetInappState);
}
__attribute__((constructor))
void lib_main() {
	// Can be called in 'main'
	std::thread(main_geokar2006_thread).detach();
}
int main() {
	return 1;
}