.. -*- coding: utf-8 -*-

.. _bgeneraltask_rst:

=============
BGeneralTask
=============

BGeneralTask 继承自BAbstractTask，提供通用的任务模型。:ref:`更多.. <bgeneraltask_detailed_description>`

========    ===================================================================
属性         值
========    ===================================================================
头文件       #include <BGeneralTask.h>
最低版本      0.1.1-alpha
继承自       BAbstractTask
========    ===================================================================

.. Note::
    BGeneralTask中的成员函数都是线程安全的。

公有成员
========

=============       ============================================================
类型                  成员名
=============       ============================================================
..                  :ref:`BGeneralTask() <bgeneraltask>`
..                  :ref:`BGeneralTask(bool _autodestroy) <bgeneraltask>`
..                  :ref:`BGeneralTask(void* _buffer, size_t _size) <bgeneraltask>`
..                  :ref:`BGeneralTask(bool _autodestroy, void* _buffer, size_t _size) <bgeneraltask>`
virtual             :ref:`~BGeneralTask() <d_bgeneraltask>`
virtual int         :ref:`process() <process>`
=============       ============================================================

.. _bgeneraltask_detailed_description:

----------------------------
详细描述：
----------------------------
BGeneralTask 提供了所有任务类的底层抽象，任何运行于线程池中的任务都是继承自这个类。

BGeneralTask 及其派生类都应当遵循只串行化运行的原则，也就是说同一时刻只能有一个线程在访问该对象。
BGeneralTask 使用 m_task_mutex 这个互斥锁来实现这一机制，确保任意时刻只有一个线程拥有该锁，其他尝试获取该锁的线程都应该立即返回一个错误值而不是继续等待获取该锁。

.. _bgeneraltask:

-----------------
BGeneralTask()
-----------------

构建一个BGeneralTask对象。

----------------------------------
BGeneralTask(bool _autodestroy)
----------------------------------

构建一个BGeneralTask对象。参数 _autodestroy 表示是否自动析构。

当该任务被线程池（仅BGeneralThreadPool支持）执行完成后会通过 destroyable() 检查该属性，若为true线程函数会自动析构这个任务对象。

--------------------------------------------
BGeneralTask(void* _buffer, size_t _size)
--------------------------------------------

构建一个BGeneralTask对象。参数_buffer指向需要传入的数据所在的内存地址，_size表示这块内存的大小。

BGeneralTask会将该内存块的数据复制到一个新的内存区域中，由BGeneralTask来进行管理该内存块。_buffer指针在传入之后，就可以立即释放该内存地址指向的资源。

------------------------------------------------------------------
BGeneralTask(bool _autodestroy, void* _buffer, size_t _size)
------------------------------------------------------------------

构建一个BGeneralTask对象。参数_autodestroy 表示是否自动析构，_buffer指向需要传入的数据所在的内存地址，_size表示这块内存的大小。

当该任务被线程池（仅BGeneralThreadPool支持）执行完成后会通过 destroyable() 检查该属性，若为true线程函数会自动析构这个任务对象。同时BGeneralTask会将_buffer指向的数据复制到一个新的内存区域中，由BGeneralTask来进行管理该内存块。_buffer指针在传入之后，就可以立即释放该内存地址指向的资源。

.. _d_bgeneraltask:

--------------------------
virtual ~BGeneralTask()
--------------------------

析构BGeneralTask对象。

.. _process:

--------------------------
virtual int process()
--------------------------

