package priv.bajdcc.lexer.regex;

import java.util.ArrayList;

/**
 * �ַ����ϣ����ַ���Χ��״̬���飨Sigma���ϣ�
 * 
 * @see CharacterRange
 * @author bajdcc
 *
 */
public class CharacterMap implements IRegexComponentVisitor {

	/**
	 * Unicode�ַ��ķ�Χ0-65535
	 */
	private static final int g_iUnicodeMapSize = 0x10000;

	/**
	 * �����Ľ������
	 */
	private int m_iLevel = 0;

	/**
	 * ���·����ķ�Χ����
	 */
	private ArrayList<CharacterRange> m_arrRanges = new ArrayList<CharacterRange>();

	/**
	 * �����ع����淶�������ַ����伯��
	 * 
	 * @return �ع����ַ����伯��
	 */
	public ArrayList<CharacterRange> getRanges() {
		return m_arrRanges;
	}

	/**
	 * �����ַ���Unicode����״̬ӳ�����С65536
	 */
	private int[] m_arrStatus = new int[g_iUnicodeMapSize];

	/**
	 * ����Unicode�ַ�ӳ���
	 * 
	 * @return �ַ�ӳ���
	 */
	public int[] getStatus() {
		return m_arrStatus;
	}

	/**
	 * ���򷽷�
	 */
	private CharacterRangeComparator m_Comparator = new CharacterRangeComparator();

	@Override
	public void visitBegin(Charset node) {
		increaseLevel();
		if (node.m_bReverse) {
			preceedReverse(node);// ����ȡ������
		}
		addRanges(node);// ��״̬���Ϸֽ��ع�
	}

	@Override
	public void visitBegin(Constructure node) {
		increaseLevel();
	}

	@Override
	public void visitBegin(Repetition node) {
		increaseLevel();
	}

	@Override
	public void visitEnd(Charset node) {
		decreaseLevel();
	}

	@Override
	public void visitEnd(Constructure node) {
		decreaseLevel();
	}

	@Override
	public void visitEnd(Repetition node) {
		decreaseLevel();
	}

	/**
	 * ����ָ���ַ����ڵ����䷶Χ���
	 * 
	 * @param ch
	 *            �ַ�
	 * @return ��ţ�-1��������
	 */
	public int find(char ch) {
		for (int i = 0; i < m_arrRanges.size(); i++) {
			CharacterRange range = m_arrRanges.get(i);
			if (range.include(ch)) {
				return i;
			}
		}
		return -1;
	}

	/**
	 * ������ȥ�ַ����е�ȡ������
	 * 
	 * @param charset
	 *            �ַ���
	 */
	private void preceedReverse(Charset charset) {
		charset.m_bReverse = false;
		charset.m_arrPositiveBounds.sort(m_Comparator);// �Լ�������
		ArrayList<CharacterRange> ranges = new ArrayList<CharacterRange>();
		CharacterRange oldRange = new CharacterRange();
		for (CharacterRange range : charset.m_arrPositiveBounds) {
			if (range.m_chLowerBound > oldRange.m_chUpperBound + 1) {// ��ǰ�½����֮ǰ�Ͻ磬�����
				CharacterRange midRange = new CharacterRange(
						(char) (oldRange.m_chUpperBound + 1),
						(char) (range.m_chLowerBound - 1));// ��ӷ�Χ����֮ǰ�Ͻ絽��ǰ�½�
				ranges.add(midRange);
				oldRange = range;
			}
		}
		if (oldRange.m_chUpperBound < g_iUnicodeMapSize - 1) {
			CharacterRange midRange = new CharacterRange(
					(char) (oldRange.m_chLowerBound + 1),
					(char) (g_iUnicodeMapSize - 1));
			ranges.add(midRange);// ������ķ�Χ
		}
		charset.m_arrPositiveBounds = ranges;
		charset.m_arrPositiveBounds.sort(m_Comparator);// �Լ�������
	}

	/**
	 * ��ȼ�һ
	 */
	private void increaseLevel() {
		m_iLevel++;
	}

	/**
	 * ��ȼ�һ
	 */
	private void decreaseLevel() {
		m_iLevel--;
		if (m_iLevel == 0) {// �����������
			putStatus();
		}
	}

	/**
	 * �������״̬
	 */
	private void putStatus() {
		for (int i = 0; i < g_iUnicodeMapSize; i++) {
			m_arrStatus[i] = -1;// ����Ԫ����Ϊ��Ч״̬-1
		}
		for (int i = 0; i < m_arrRanges.size(); i++) {
			int lower = m_arrRanges.get(i).m_chLowerBound;
			int upper = m_arrRanges.get(i).m_chUpperBound;
			for (int j = lower; j <= upper; j++) {
				m_arrStatus[j] = i;// ����Χi�а���������Ԫ����Ϊi
			}
		}
	}

	/**
	 * ��������ӵ��ַ���Χ����Ҫʱ����ֽ⣬ʹ��Ԫ�ؼ��໥����
	 * 
	 * @param CharacterRange
	 *            �ַ�����
	 */
	private void addRange(CharacterRange newRange) {

		for (int i = 0; i < m_arrRanges.size(); i++) {
			m_arrRanges.sort(m_Comparator);
			CharacterRange oldRange = m_arrRanges.get(i);
			/*
			 * ��ֹ��������[New]��֮ǰ����[Old]�������������У��򽫼��Ϸ���
			 */
			if (oldRange.m_chLowerBound < newRange.m_chLowerBound) {
				if (oldRange.m_chUpperBound < newRange.m_chLowerBound) {

					// [####Old####]_______________
					// ______________[#####New####]
					// [Old]��[New]С��û�н���

				} else if (oldRange.m_chUpperBound < newRange.m_chUpperBound) {

					// [######Old######]__________
					// ______________[#####New####]
					// [Old]��[New]�н���[New.Lower,Old.Upper]
					// [Old]=[Old.Lower,New.Lower-1]
					// [Mid]=[New.Lower,Old.Upper]
					// [New]=[Old.Upper+1,New.Upper]

					m_arrRanges.remove(i);
					newRange.m_chLowerBound = (char) (oldRange.m_chUpperBound + 1);
					oldRange.m_chUpperBound = (char) (newRange.m_chLowerBound - 1);
					m_arrRanges.add(oldRange);
					m_arrRanges.add(new CharacterRange(
							(char) (oldRange.m_chUpperBound + 1),
							(char) (newRange.m_chLowerBound - 1)));
					i++;
				} else if (oldRange.m_chUpperBound == newRange.m_chUpperBound) {

					// [###########Old############]
					// ______________[#####New####]
					// [Old]��[New]�н���[New]
					// [Old]=[Old.Lower,New.Lower-1]
					// [New]=[New]

					m_arrRanges.remove(i);
					oldRange.m_chUpperBound = (char) (newRange.m_chLowerBound - 1);
					m_arrRanges.add(newRange);
					m_arrRanges.add(oldRange);
					return;
				} else {

					// [#############Old##############]
					// ______________[#####New####]____
					// [Old]��[New]�н���[New]
					// [Left]=[Old.Lower,New.Lower-1]
					// [Mid]=[New]
					// [Right]=[New.Upper+1]

					m_arrRanges.remove(i);
					m_arrRanges.add(new CharacterRange(oldRange.m_chLowerBound,
							(char) (newRange.m_chUpperBound - 1)));
					m_arrRanges.add(newRange);
					m_arrRanges.add(new CharacterRange(
							(char) (newRange.m_chLowerBound + 1),
							oldRange.m_chUpperBound));
					return;
				}
			} else if (oldRange.m_chLowerBound == newRange.m_chLowerBound) {
				if (oldRange.m_chUpperBound < newRange.m_chUpperBound) {

					// [#######Old#######]
					// [##########New##########]
					// [Old]��[New]�н���[Old]

					newRange.m_chLowerBound = (char) (oldRange.m_chUpperBound + 1);
				} else if (oldRange.m_chUpperBound == newRange.m_chUpperBound) {

					// [#######Old#######]
					// [#######New#######]
					// [Old]=[New]

					return;
				} else {

					// [##########Old##########]
					// [#######New#######]
					// [Old]��[New]�н���[New]
					// [Old]=[New.Upper+1,Old.Upper]
					// [New]=[New]

					m_arrRanges.remove(i);
					oldRange.m_chLowerBound = (char) (newRange.m_chUpperBound + 1);
					m_arrRanges.add(newRange);
					m_arrRanges.add(oldRange);
					return;
				}
			} else if (oldRange.m_chLowerBound <= newRange.m_chUpperBound) {
				if (oldRange.m_chUpperBound < newRange.m_chUpperBound) {

					// ___[#######Old#######]___
					// [##########New##########]
					// [Old]��[New]�н���[Old]
					// [Left]=[New.Lower,Old.Lower-1]
					// [New]=[Old.Upper+1,New.Upper]

					m_arrRanges.add(new CharacterRange(newRange.m_chLowerBound,
							(char) (oldRange.m_chLowerBound - 1)));
					newRange.m_chLowerBound = (char) (oldRange.m_chUpperBound + 1);
					i++;
				} else if (oldRange.m_chUpperBound == newRange.m_chUpperBound) {

					// ______[#######Old#######]
					// [##########New##########]
					// [Old]��[New]�н���[Old]
					// [Old]=[Old]
					// [New]=[New.Lower,Old.Lower-1]

					newRange.m_chUpperBound = (char) (oldRange.m_chLowerBound - 1);
					m_arrRanges.add(newRange);
					return;
				} else {

					// ______[##########Old##########]
					// [##########New##########]______
					// [Old]��[New]�н���[Old.Lower,New.Upper]
					// [Old]=[New.Upper+1,Old.Upper]
					// [Mid]=[Old.Lower,New.Upper]
					// [New]=[New.Lower,Old.Lower-1]

					m_arrRanges.remove(i);
					newRange.m_chUpperBound = (char) (oldRange.m_chLowerBound - 1);
					oldRange.m_chLowerBound = (char) (newRange.m_chUpperBound + 1);
					m_arrRanges.add(oldRange);
					m_arrRanges.add(new CharacterRange(oldRange.m_chLowerBound,
							newRange.m_chUpperBound));
					m_arrRanges.add(newRange);
					return;
				}
			}
		}
		m_arrRanges.add(newRange);
	}

	/**
	 * ��������ӵ��ַ���Χ����Ҫʱ����ֽ⣬ʹ��Ԫ�ؼ��໥����
	 * 
	 * @param charset
	 *            �ַ���
	 */
	private void addRanges(Charset charset) {
		for (CharacterRange range : charset.m_arrPositiveBounds) {
			addRange(new CharacterRange(range.m_chLowerBound, range.m_chUpperBound));
		}
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		for (CharacterRange range : m_arrRanges) {
			sb.append(range + System.getProperty("line.separator"));
		}
		return sb.toString();
	}
}
