﻿#include <object_arx_global.hpp>
#include "UpdateAll.hpp"

/*****************************************************/
#include "../UpdateBlockFromOtherFile/UpdateBlockFromOtherFile.hpp"
#include "../UpdateLayer/UpdateLayer.hpp"
#include "../UpdateTextStyle/UpdateTextStyle.hpp"
#include "../UpdateDimStyle/UpdateDimStyle.hpp"
#include "../UpdateMLeaderStyle/UpdateMLeaderStyle.hpp"
#include "../UpdateTitleBlockTime/UpdateTitleBlockTime.hpp"
#include "../CopyFilePathToClipboard/CopyFilePathToClipboard.hpp"
/*****************************************************/

sstd::UpdateAll::UpdateAll() {
}
namespace sstd {
	namespace {
		class ThisFunction {
			AcDbDatabase * const $DB;
		public:
			inline ThisFunction( AcDbDatabase * arg ):$DB(arg) {}
			inline void updateAll();
		};
		inline void ThisFunction::updateAll() {
			UpdateTextStyle::main();
			UpdateBlockFromOtherFile::main();
			UpdateDimStyle::main();
			UpdateMLeaderStyle::main();
			UpdateTitleBlockTime::main();
			UpdateLayer::main();
			CopyFilePathToClipboard::main();
			{
				auto DB = acdbHostApplicationServices()->workingDatabase();
				DB->setDimAssoc(2)/*标注关联*/;
				DB->setAuprec(2)/*角度精度*/;
				DB->setLuprec(2)/*线性精度*/;
				DB->setLunits(2)/*线性小数*/;
				DB->setAunits(0)/*十进制角度*/;
				DB->setPdmode(35)/*设置点样式*/;
				DB->setPdsize(3)/*设置点样式*/;
				{/**
				 http://jprdintprev.autodesk.com/adn/servlet/devnote?siteID=4814862&id=5414556&preview=1&linkID=4900509
				 **/
					AcDbDatabaseSummaryInfo *varInfo = nullptr;
					acdbGetSummaryInfo(DB, varInfo);
					auto varCTime = DB->tdcreate();

					auto toWstring = [](short arg) {
						return std::to_wstring(arg);
					};								

					if (varInfo) {
						{/*设置关键字*/
							wchar_t * varInfoValue = nullptr;
							varInfo->getKeywords(varInfoValue);
							if ((varInfoValue == nullptr) || (0 == std::wcslen(varInfoValue))) {
								std::wstring varDataTime;
								varDataTime = toWstring(varCTime.year());
								varDataTime += LR"(-)"sv;
								varDataTime += toWstring(varCTime.month());
								varDataTime += LR"(-)"sv;
								varDataTime += toWstring(varCTime.day());
								varInfo->setKeywords(varDataTime.c_str());
							}
							acutDelString(varInfoValue);
						}
						{/*设置作者*/
							varInfo->setAuthor(LR"(LZLT)");
						}
					}
					
					acdbPutSummaryInfo(varInfo);
				}
			}
			auto varCD = acDocManager->curDocument();
			if (varCD) {
				acDocManager->sendStringToExecute(
					varCD,
					LR"(vvvvvvattsyngriducsicon
)"
				);
			}
		}
	}/*namespace*/
}/*namespace sstd*/

void sstd::UpdateAll::load() {
	arx_add_main_command<UpdateAll>();
}

void sstd::UpdateAll::main() {
	ThisFunction varFunction( acdbHostApplicationServices()->workingDatabase() );
	varFunction.updateAll();
}

namespace sstd {
	extern void loadUpdateAll() { UpdateAll::load(); }
}/*namespace*/

