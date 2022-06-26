;acquireLock:
;.retry:
;    lock bts [lock], 0
;    jc .retry
;    ret
;
;releaseLock:
;    lock btr [lock], 0
;    ret
;