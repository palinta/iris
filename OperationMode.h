#ifndef OperationMode_h
#define OperationMode_h

enum OperationMode {
    RAINBOW,
    
    // Publishes an public function, when 
    CONFIGURABLE_MODE,
};

OperationMode operationMode = RAINBOW;

void SetOperationMode (OperationMode mode);

#endif // OperationMode_h

