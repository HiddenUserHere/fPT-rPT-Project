/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef MYGUI_DIRECTX_VERTEX_BUFFER_H_
#define MYGUI_DIRECTX_VERTEX_BUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_DirectXRenderManager.h"

struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;

namespace MyGUI
{

	class DirectXVertexBuffer : public IVertexBuffer
	{
	public:
		DirectXVertexBuffer(IDirect3DDevice9* _device, DirectXRenderManager* _pRenderManager);
		~DirectXVertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() override;

		Vertex* lock() override;
		void unlock() override;

	/*internal:*/
		virtual bool setToStream(size_t stream);

	private:
		void create();
		void destroy();
		void resize();

	private:
		IDirect3DDevice9* mpD3DDevice;
		IDirect3DVertexBuffer9* mpBuffer;
		DirectXRenderManager* pRenderManager;

		size_t mVertexCount;
		size_t mNeedVertexCount;
	};

} // namespace MyGUI

#endif // MYGUI_DIRECTX_VERTEX_BUFFER_H_
