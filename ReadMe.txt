char[]형인 pTmp를 memset으로 '\0' 넣고 DB에서 가져온 데이터를 삽입해서 전송

OnReceive에서 그걸 분리해 사용함.

View,Doc구조에서는 View에서 핸들처리하는게 편함(메시지맵이 잘돌아서?,다중문서는 아직 모르겠음)

만들어진 Stand Alone View,Doc 구조에 서버를 넣고, 동적할당시키고 DB까지 연동시킴.

이미지 부분만 개선하면 될 것