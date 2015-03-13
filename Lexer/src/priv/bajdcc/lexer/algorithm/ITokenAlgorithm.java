package priv.bajdcc.lexer.algorithm;

/**
 * ���/��ȡ���ʵ��㷨�ӿ�
 * 
 * @author bajdcc
 */
public interface ITokenAlgorithm {

	/**
	 * ��ǰ�㷨�Ƿ������ӦVisitor���󣨼��Ƿ�ƥ�䣩
	 * 
	 * @param visitor
	 *            �ַ�����������
	 * @return �㷨ƥ����
	 */
	boolean accept(ITokenVisitor visitor);
}
