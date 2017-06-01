package com.adjust.testlibrary;

public class AdjustTesting2dxCommandCallback implements ICommandJsonListener {
	public native void commandExecuted(String className, String methodName, String jsonParameters);
	
	public AdjustTesting2dxCommandCallback() {}
	
	@Override
    public void executeCommand(String className, String methodName, String jsonParameters) {
		commandExecuted(className, methodName, jsonParameters);
	}
}
