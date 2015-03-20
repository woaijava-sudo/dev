package priv.bajdcc.lexer.regex;

import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.error.RegexException.RegexError;
import priv.bajdcc.lexer.token.MetaType;
import priv.bajdcc.lexer.utility.Position;

/**
 * �ַ����������ӿ�
 * 
 * @author bajdcc
 *
 */
public interface IRegexStringIterator {

	/**
	 * �׳�����
	 * 
	 * @param error
	 *            ��������
	 * @throws RegexException
	 */
	public abstract void err(RegexError error) throws RegexException;

	/**
	 * ������һ���ַ�
	 */
	public abstract void next();

	/**
	 * ���뵱ǰ�ַ�
	 */
	public abstract void translate();

	/**
	 * �жϵ�ǰλ�ò���ĩβ
	 * 
	 */
	public abstract boolean available();

	/**
	 * ǰ��һ���ַ���look forward��
	 * 
	 */
	public abstract void advance();

	/**
	 * ��õ�ǰ�ַ�
	 * 
	 */
	public abstract char current();

	/**
	 * ����ַ�����
	 */
	public abstract MetaType meta();

	/**
	 * ��õ�ǰλ��
	 */
	public abstract int index();

	/**
	 * ��õ�ǰλ��
	 */
	public abstract Position position();

	/**
	 * ȷ�ϵ�ǰ�ַ�
	 * 
	 * @param meta
	 *            ����
	 * @param error
	 *            �׳��Ĵ���
	 * @throws RegexException
	 */
	public abstract void expect(MetaType meta, RegexError error)
			throws RegexException;

	/**
	 * ���浱ǰλ��
	 */
	public abstract void snapshot();

	/**
	 * ���ǵ�ǰλ��
	 */
	public abstract void cover();

	/**
	 * �ָ����ϴ�λ��
	 */
	public abstract void restore();

	/**
	 * �����ϴ�λ��
	 */
	public abstract void discard();

	/**
	 * ��ý������
	 */
	public abstract RegexStringUtility utility();
}