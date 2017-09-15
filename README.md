# SOReloader
A framework for update shared library runtime. You don't need to stop the program any more!

## Create your own shared library
If you want to export a class and use it out of library, you should follow these instructions:

> 1.    You must implement the **static ClassName()** function in the .h file. Return the name or class.
> Example:
> ```C++
> class Player
> {
> public :
>   static std::string ClassName() { return "Player"; }
> };
> ```

> 2.    Add **OFFER_CREATE_DESTROY_FUNCTION(…)** macro at the end of the class your want to export. This macro offer the function to create object and destroy object. This macro in the MacroDefine.h.
> Example:
> ```C++
> class Player
> {
>   ...
> }
> OFFER_CREATE_DESTROY_FUNCTION(Player)
> ```

> 3.    The destructor of the class must add **virtual** keyword, and the destructor must be implemented in the .h file. If destruct object is complicated, you can write the logic in a private member function. Then call it in the destructor.
> Example:
> ```C++
> class Player
> {
> public:
>   Player();
>   virtual ~Player() { destroy(); }
> private:
>	void destroy();
> }
> ```

> 4.    All of member functions that you want to use it out of library must add **virtual** keyword.
> Example:
> ```C++
> class Player
> {
> public:
>   virtual void AddGold(int gold);
> }
> ```

> 5.    If want just export an normal function in the library, you must declare it with **extern "C"**.
> Example:
> ```C++
> extern "C"
> {
>   Player *CreatePlayer()
>   {
>       return new Player;
>   }
>   void DestroyPlayer(Player *object)
>   {
>       if (object != nullptr)
>       {
>           delete object;
>       }
>   }
> }
> ```

> 6.    With SOReloader, you can make every moudle a single shared library. It easier to update every moudle and don't need to stop your program. **Notice: the library you create must link the SOReloader.so**

## Use your own created shared library
There are also some instructions you need to follow:

> 1.    If you want to create an object and it's type are defined in the shared library you created before. You can't create it like usual. The only way to create it out of the library is call
> ```C++
> template<typename T>
> T *CreateSharedObject(SharedLibrary *lib)
> ```
> in the **SharedObjectLifeController.h**.

> 2.    You must delete the object by yourself. And you can put the object in the **std::shared_ptr**, let std::shared_ptr delete the object. Otherwise it will case memory leak.

> 3.    If you want to use **std::shared_ptr** control the object when to delete, you can't use **std::make_shared\<T>(T&)** create a std::shared_ptr. You need use std::shared_ptr's constructor **std::shared_ptr\<T>(T\*)** to create it.

> 4.    If you want to call a common function exported in the library, you need declare the function type. Becasue **SharedLibrary::GetFunction(std::string funcName)** returns a **void \***. You need convert this pointer to the correct function type.
> Example:
> ```C++
> #inlcude <SOReloader/SharedLibrary.h>
> 
> SharedLibrary playerLib("Player.so");
> typedef Player * CreatePlayerFunc();
> CreatePlayerFunc * pFunc = (CreatePlayerFunc*)playerLib.GetFunction("CreatePlayer");
> Player *player = pFunc();
> ```

## Demo
All instructions serve for runtime update. It makes you can update logic when program is running. Here is a small demo.

> ### Player.so & PlayerUpdated.so

> Player.h
> ```C++
> #include <cstdint>
> #include "SOReloader/MacroDefine.h"
> #include <string>
> #include <iostream>
> 
> class Player
> {
> public :
>	Player();
>	virtual ~Player() { destroy();}
>   virtual void AddGold(uint64_t gold);
>   static std::string ClassName() { return "Player"; }
>
> private :
>	void destroy();
>
> private :
>   uint64_t mGold;
> };
> OFFER_CREATE_DESTROY_FUNCTION(Player)
> ```

> Player.cpp
> ```C++
> #include "Player.h"
> 
> Player::Player()
> {
>	mGold = 0;
> }
>
> void Player::destroy()
> {
> 	
> }
>
> void Player::AddGold(uint64_t gold)
> {
>	std::cout << "version 1 add gold: " << gold << "extra gold: " << "0" <<std::endl;
>	mGold += gold ;
>   /* rebuild Player.so and replace these logic
>	std::cout << "version 2 add gold: " << gold << "extra gold: " << "100" <<std::endl;
>	mGold += gold + 100;
>   */
> }
> ```
> Now, prepare two shared library, Player.so & PlayerUpdated.so

> ### Test Project

> main.cpp
> ```C++
> #include <Player/Player.h>
> #include <SOReloader/SharedObjectLifeController.h>
> #include <SOReloader/SharedLibrary.h>
> #include <SOReloader/SOManager.h>
>
> int main(int argc, char *argv[])
> {
>   SOManager manager;
>
>   //load shared library in config to program
>   //manager.Initialize("SharedLibraryConfig.xml");
>
>   manager.UpdateSharedLibrary("Player", "Player.so");//load Player.so
>   {// first version
>       SharedLibrary *playerLib = manager.GetLibrary("SOPlayer");
>       std::shared_ptr<Player> spPlayer(CreateSharedObject<Player>(playerLib));
>       spPlayer->AddGold(100);
>   }
>
>   manager.UpdateSharedLibrary("Player", "PlayerUpdate.so");//Update the Player.so with PlayerUpdate.so
>   {// second version
>       SharedLibrary *playerLib = manager.GetLibrary("SOPlayer");
>       std::shared_ptr<Player> spPlayer(CreateSharedObject<Player>(playerLib));
>       spPlayer->AddGold(100);
>   }
>   
>   {// rollback to previous version
>       manager.RollbackLibrary("Player");
>       SharedLibrary *playerLib = manager.GetLibrary("SOPlayer");
>       std::shared_ptr<Player> spPlayer(CreateSharedObject<Player>(playerLib));
>       spPlayer->AddGold(100);
>   }
> }
> ```

> Output:
> version 1 add gold: 100 extra gold: 0
> version 2 add gold: 100 extra gold: 100
> version 1 add gold: 100 extra gold: 0

> ## End & Notice
> If you update the interface, you can't update the .so file directly. You need restart your program. Such as void Player::AddGold(uint64_t gold) update to bool Player::AddGold(uint64_t gold, bool force), it may crash your program. There is another choice that you can add a new interface to your shared library, update the library without stoping program. And you can use the new interface in other library.
