package priv.bajdcc.lexer.utility;

import org.vibur.objectpool.PoolObjectFactory;

/**
 * �����
 * 
 * @author bajdcc
 * @param T
 *            ��������
 */
public abstract class ObjectFactory<T> implements PoolObjectFactory<T> {

	public boolean readyToTake(T obj) {
		return true;
	}

	public boolean readyToRestore(T obj) {
		return true;
	}

	public void destroy(T obj) {

	}
}
