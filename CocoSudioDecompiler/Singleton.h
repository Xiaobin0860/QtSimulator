//
//  Singleton.h
//  game
//
//  Created by Xiaobin Li on 9/20/14.
//
//

#ifndef game_Singleton_h
#define game_Singleton_h

#define DECLARE_SINGLETON(name) friend class GameBase::Singleton<name>;

namespace GameBase {
    
    /*!
	 *
     * @class Singleton 单例模板
     * @note 简单的单例模式，非线程安全。
        1. 要求程序中所有使用的单例都统一顺序初始化,
     程序结束前调用Deinit(), 使得单例初始化、销毁顺序可控
        XXX->Instance()->Init();
        ...
     
        结束
        XXX->Instance()->Deinit();
     
        2. 子类可重写doInit，doDeinit实现自己的初始化，销毁方法
     */
    template <class T>
    class Singleton {
    public:
        static T* Instance() {
            static T _instance;
            return &_instance;
        }
        
        // 这里故意与cocos命名方式不同，防止继承自cocos类以此模板实现单例发生命名冲突
        bool Init() {
			if (_inited) return true;

            if (doInit()) {
                _inited = true;
                return true;
            }

            return false;
        }

		bool IsInited() { return _inited; }
        
        void Deinit() {
            if (!_inited) return;
            _inited = false;
            doDeinit();
        }
        
    protected:
        Singleton() : _inited(false) {}
        // xxx(lxb): 这里没有调用Deinit，因为析构里调virtual函数没意义
        // 这里无法避免使用者忘记调Deinit()
        // 所以这里只是标记一下此单例已无效
        virtual ~Singleton() { _inited = false; }
        Singleton(const Singleton<T>&);
        Singleton<T>& operator=(const Singleton<T>&);
        
        bool _inited;
        
    private:
        virtual bool doInit() { return true; }
        virtual void doDeinit() {}
    };
};

#endif
