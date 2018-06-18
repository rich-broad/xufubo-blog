# 概述
随着互联网的发展，服务需要处理的请求越来越多，传统的单机已经无法满足需求，而且互联网业务的特点往往要求服务快速扩容，如此这些特点，使得现在的后台架构越来越复杂。完全从单机演化到分布式系统。分布式系统常常使用RPC作为其通信基础，RPC与传统的单机版过程/函数调用不同，传统的单机函数调用，不是成功就是失败，而RPC却不只是是与非的问题，它又引入了第三态，超时(timeout)，超时的情况下，可能成功，也可能失败，换句话说，RPC的结果是未知的，超时情况下，可能会重试，这时候，接口的幂等性就是非常重要的了。  
为了理解接口的幂等性，我们先举一个例子：  
对于一个电商系统，常常会涉及到扣减库存的问题，场景如下：  
 - 1、买家购买商品，需要减少库存，这时调用RPC接口；  
 - 2、由于网络问题、负载过高等，接口超时了；  
 - 3、看到接口超时了，调用方（可以是服务重试逻辑，也可以是买家触发）再次调用；  
 - 4、负载降下来了，或者网络恢复了，这时，接口被执行了两次；  
 - 5、于是虽然只是买了一件商品，库存却减少了两次。  
由此，可以折射出分布式系统设计中，由于其三态性(成功、失败、超时)导致的一些问题：  
 - 1、减少库存的接口不符合幂等性；  
 - 2、服务设计也不合理，该服务应该丢掉过早的消息，同时进行过载保护，以避免雪崩（这个话题不在此讨论）。  
对于以上问题的第一点，是正确性的问题，第一点必须保证，其实就是减少库存的接口不符合[幂等](https://en.wikipedia.org/wiki/Idempotence)性。  
所谓幂等，就是：f(f(x)) = f(x)。在软件工程领域，它的语义是：函数/接口可以使用相同的参数重复执行，不管执行多少次，对系统状态的影响就像是调用了一次。对于只读的接口，天然幂等，对于有写的接口，因为在分布式系统中常常遇到各种不可靠问题（网络、机器、机架、机房故障，等等）而需要重试，这时，在设计接口时就加入 **版本号** 或 **序列号** 之类的机制以拒绝已经发生的过程，保证幂等。  
下边我们就研究下如何设计具有幂等性接口的函数/接口。  
现在假设这个接口的名字为sub_stock。我们如何完善接口的幂等性呢？这里借鉴金融系统的做法，引入 **票据(token)** 是个不错的主意：  
 - 1、减少库存前，调用方先获取一个交易票据token(这就是我们常见的订单号)；  
 - 2、被调方生成唯一的token并记录到DB中，并将该token标记为未执行；  
 - 3、调用方拿到token之后去减少库存（不仅需要库存减少的数量，还需要token作为参数），sub_stock(token, sub_num)；  
 - 4、但是这时由于各种原因（例如网络拥塞），被调方没来得及返回，超时了；  
 - 5、然后调用方重试；  
 - 6、被调方收到第二次sub_stock(token, sub_num)，不过这时被调方看到该token已经被执行了，那么这时被调方什么也不会做，只要返回就好了。  
 > - 当然了，sub_stock(token, sub_num)在执行时，将验证token、标记为已执行，和扣减库存这三步是原子操作。  
可见在分布式系统中生成一个全局唯一的id是非常重要的，如下介绍几种：  
 - 1、利用业务固有属性，例如订单号+uid；  
 - 2、利用机器特性：ip + 进程/线程id + timeval的时间戳 + 随机数；  
 - 3、实现一个微服务用于生成id，所有的服务都访问它，不过容易造成性能瓶颈。  