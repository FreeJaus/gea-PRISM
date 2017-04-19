#pragma once

class Engine final : public Singleton<Engine> {

	friend class Singleton<Engine>;

private:

	static void RunCallBack();

	Engine() : Singleton<Engine>() {}

	class Injector final : public Singleton<Injector> {

		friend class Singleton<Injector>;

	private:

		bool IsInjected();
		DWORD GetProcessIdByExeName(LPWSTR szExeName);

		Injector() : Singleton<Injector>() {}

	public:

		void LoadDLL();
		void UnloadDLL();

	};

public:

	void Run();
	void Stop();

};