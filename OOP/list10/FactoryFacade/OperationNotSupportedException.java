package FactoryFacade;

public class OperationNotSupportedException extends Exception {

    public OperationNotSupportedException() {
        super("Operation not supported");
    }
}
