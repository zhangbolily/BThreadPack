.. -*- coding: utf-8 -*-

.. _babstracttask_rst:

=============
BAbstractTask
=============

BAbstractTask 提供了所有任务类的底层抽象，任何运行于线程池中的任务都是继承自这个类。:ref:`更多.. <babstracttask_detailed_description>`

========    ===================================================================
属性         值
========    ===================================================================
头文件       #include <BAbstractTask.h>
最低版本      0.1.1-alpha
被继承       :ref:`BGeneralTask <bgeneraltask_rst>`
========    ===================================================================

.. Note::
    BAbstractTask中的成员函数都是线程安全的。
    
公有类型
========

========    ===================================================================

========    ===================================================================
enum        BTaskStatus { TaskFailed, TaskInit, TaskExecuting, TaskFinished }
========    ===================================================================

公有成员
========

=========       ============================================================
类型              成员名
=========       ============================================================
..              :ref:`BAbstractTask() <babstracttask>`
..              :ref:`BAbstractTask(bool _autodestroy) <babstracttask>`
..              :ref:`BAbstractTask(void* _buffer, size_t _size) <babstracttask>`
..              :ref:`BAbstractTask(bool _autodestroy, void* _buffer, size_t _size) <babstracttask>`
virtual         :ref:`~BAbstractTask() <d_babstracttask>`
bool            :ref:`destroyable() <destroyable>`
int             :ref:`inputBuffer(void** _buffer, size_t &_size) <inputbuffer>`
mutex           m_task_mutex
int             :ref:`outputBuffer(void** _buffer, size_t &_size) <outputbuffer>`
int             :ref:`setInputBuffer(void* _buffer, size_t _size) <setinputbuffer>`
int             :ref:`setOutputBuffer(void* _buffer, size_t _size) <setoutputbuffer>`
int             :ref:`setStatus(BTaskStatus _status) <setstatus>`
int             :ref:`setStatus(atomic_int _status) <setstatus>`
int             :ref:`status() <status>`
=========       ============================================================

.. _babstracttask_detailed_description:

----------------------------
详细描述：
----------------------------
BAbstractTask 提供了所有任务类的底层抽象，任何运行于线程池中的任务都是继承自这个类。

BAbstractTask 及其派生类都应当遵循只串行化运行的原则，也就是说同一时刻只能有一个线程在访问该对象。
BAbstractTask 使用 m_task_mutex 这个互斥锁来实现这一机制，确保任意时刻只有一个线程拥有该锁，其他尝试获取该锁的线程都应该立即返回一个错误值而不是继续等待获取该锁。

.. _babstracttask:

-----------------
BAbstractTask()
-----------------

构建一个BAbstractTask对象。

----------------------------------
BAbstractTask(bool _autodestroy)
----------------------------------

构建一个BAbstractTask对象。参数 _autodestroy 表示是否自动析构。

当该任务被线程池（仅BGeneralThreadPool支持）执行完成后会通过 destroyable() 检查该属性，若为true线程函数会自动析构这个任务对象。

--------------------------------------------
BAbstractTask(void* _buffer, size_t _size)
--------------------------------------------

构建一个BAbstractTask对象。参数_buffer指向需要传入的数据所在的内存地址，_size表示这块内存的大小。

BAbstractTask会将该内存块的数据复制到一个新的内存区域中，由BAbstractTask来进行管理该内存块。_buffer指针在传入之后，就可以立即释放该内存地址指向的资源。

------------------------------------------------------------------
BAbstractTask(bool _autodestroy, void* _buffer, size_t _size)
------------------------------------------------------------------

构建一个BAbstractTask对象。参数_autodestroy 表示是否自动析构，_buffer指向需要传入的数据所在的内存地址，_size表示这块内存的大小。

当该任务被线程池（仅BGeneralThreadPool支持）执行完成后会通过 destroyable() 检查该属性，若为true线程函数会自动析构这个任务对象。同时BAbstractTask会将_buffer指向的数据复制到一个新的内存区域中，由BAbstractTask来进行管理该内存块。_buffer指针在传入之后，就可以立即释放该内存地址指向的资源。

.. _d_babstracttask:

--------------------------
virtual ~BAbstractTask()
--------------------------

析构BAbstractTask对象。

.. _destroyable:

--------------------------
bool destroyable()
--------------------------

返回 _autodestroy 的属性值，如果为true表示该对象可以被主动析构。

.. _inputbuffer:

----------------------------------------------------
int inputBuffer(void** _buffer, size_t &_size)
----------------------------------------------------

获取任务的输入数据。

该函数返回 BThreadPack::ReturnCode::BOnlySingleThread 表示该BAbstractTask对象正被多个线程共享。其他情况下均返回 BThreadPack::ReturnCode::BSuccess。

.. _outputbuffer:

----------------------------------------------------
int outputBuffer(void** _buffer, size_t &_size)
----------------------------------------------------

获取任务的输出数据。

该函数返回 BThreadPack::ReturnCode::BOnlySingleThread 表示该BAbstractTask对象正被多个线程共享。其他情况下均返回 BThreadPack::ReturnCode::BSuccess。

.. _setinputbuffer:

----------------------------------------------------
int setInputBuffer(void* _buffer, size_t _size)
----------------------------------------------------

设置任务的输入数据。参数_buffer指向需要传入的数据所在的内存地址，_size表示这块内存的大小。

返回 BThreadPack::ReturnCode::BOnlySingleThread 表示该BAbstractTask对象正被多个线程共享。返回 BThreadPack::ReturnCode::BError 表示输入参数有误。其他情况下均返回 BThreadPack::ReturnCode::BSuccess。

.. _setoutputbuffer:

----------------------------------------------------
int setOutputBuffer(void* _buffer, size_t _size)
----------------------------------------------------

设置任务的输出数据。参数_buffer指向需要输出的数据所在的内存地址，_size表示这块内存的大小。

返回 BThreadPack::ReturnCode::BOnlySingleThread 表示该BAbstractTask对象正被多个线程共享。返回 BThreadPack::ReturnCode::BError 表示输入参数有误。其他情况下均返回 BThreadPack::ReturnCode::BSuccess。

.. _setstatus:

----------------------------------------------------
int setStatus(BTaskStatus _status)
----------------------------------------------------

设置任务的状态。

-----------------------------------
int setStatus(atomic_int _status)
-----------------------------------

设置任务的状态。

.. _status:

-----------------------------------
int status()
-----------------------------------

返回任务的状态。
